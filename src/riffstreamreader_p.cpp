/*
    This file is part of the Koralle library, made within the KDE community.

    Copyright 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "riffstreamreader_p.h"

#include <QtCore/QDebug>

namespace Koralle
{

static const FourCharCode riffId('R','I','F','F');
static const FourCharCode listId('L','I','S','T');


RiffStreamReaderPrivate::RiffStreamReaderPrivate( QIODevice* device )
  : mDevice( device )
  , mDeviceStartOffset( device ? device->pos() : -1 )
  , mCurrentChunkHeader()
  , mIsCurrentChunkList( false )
  , mIsCurrentChunkFile( false )
  , mIsCurrentChunkDataRead( true )
  , mCurrentChunkData()
  , mCurrentChunkOffset( 0 )
  , mHasError( device == 0 )
  , mErrorString()
{
    // TODO: set error string for no device

    if( device && device->isSequential() )
    {
        mHasError = true;
//         mErrorString = "";
    }
}

void
RiffStreamReaderPrivate::setDevice( QIODevice* device )
{
    mDevice = device;

    resetState();

    mDeviceStartOffset = (mDevice ? mDevice->pos() : -1);

    if( device && device->isSequential() )
    {
        mHasError = true;
//         mErrorString = "";
    }
}

void
RiffStreamReaderPrivate::resetState()
{
    mCurrentChunkHeader.clear();
    mIsCurrentChunkList = false;
    mIsCurrentChunkFile = false;
    mIsCurrentChunkDataRead = true;
    mCurrentChunkData.clear();
    mCurrentChunkOffset = 0;
    mHasError = (mDevice == 0);
    mErrorString.clear();
    // TODO: set error string for no device
}


bool
RiffStreamReaderPrivate::readChunkHeader()
{
    if( mHasError )
        return false;
// qDebug() << "readChunkHeader begin: pos " << mCurrentChunkOffset<<mIsCurrentChunkDataRead;

    // last chunk data not yet passed?
    if( ! mIsCurrentChunkDataRead )
    {
        // jump to next chunk
        qint64 chunkSize = mCurrentChunkHeader.mSize;
        // has pad byte?
        if( chunkSize & 1 )
            chunkSize += 1;
        // adapt to use content
        if( mIsCurrentChunkFile || mIsCurrentChunkList )
            chunkSize -= sizeof(FourCharCode);

        mCurrentChunkOffset += chunkSize;

        if( ! mDevice->seek(mDeviceStartOffset + mCurrentChunkOffset) )
        {
            mHasError = true;
            mErrorString = mDevice->errorString();
            return false;
        }
    }

    // check if at end of list
    if( ! mListStack.isEmpty() )
    {
// qDebug() << "readChunkHeader: pos " << mCurrentChunkOffset<<"at listend"<<mListStack.top().mEndOffset;
        // nothing more to read in this list?
        if( mCurrentChunkOffset == mListStack.top().mEndOffset )
            return false;
    }

    // clear old data
    mCurrentChunkHeader.clear();
    mCurrentChunkData.clear();
    mIsCurrentChunkDataRead = false;

    // read next
    const int dataRead = mDevice->read( mCurrentChunkHeader.data(), sizeof(ChunkHeader) );

// qDebug() << "readChunkHeader:" << mCurrentChunkHeader.mId.toString() << mCurrentChunkHeader.mSize << (dataRead == sizeof(ChunkHeader));

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    {
        // TODO: is there no special function to reverse the byte order?
        char* const sizeData = reinterpret_cast<char*>(&mCurrentChunkHeader.mSize);
        char helper;
        helper = sizeData[0]; sizeData[0] = sizeData[3]; sizeData[3] = helper;
        helper = sizeData[1]; sizeData[1] = sizeData[2]; sizeData[2] = helper;
    }
#endif

    if( dataRead == sizeof(ChunkHeader) )
    {
        mCurrentChunkOffset += sizeof(ChunkHeader);

        // check if list
        mIsCurrentChunkList = ( mCurrentChunkHeader.mId == listId );
        mIsCurrentChunkFile = ( mCurrentChunkHeader.mId == riffId );

        if( mIsCurrentChunkFile || mIsCurrentChunkList )
        {
            // read list/file id
            const int dataRead = mDevice->read( mCurrentChunkHeader.mId.data(), sizeof(FourCharCode) );
// qDebug() << "read list id:" << mCurrentChunkHeader.mId.toString() << (dataRead == sizeof(FourCharCode));
            if( dataRead == sizeof(FourCharCode) )
            {
                mCurrentChunkOffset += sizeof(FourCharCode);
                // list is not re
                mIsCurrentChunkDataRead = true;
            }
            else
            {
                mHasError = true;
                mErrorString = mDevice->errorString();
            }
        }
        else
        {
            // TODO: check if there is a container list, should not be allowed?
        }
    }
    else
    {
        mHasError = true;
        mErrorString = mDevice->errorString();
    }

    return (! mHasError);
}


QByteArray
RiffStreamReaderPrivate::chunkData()
{
   if( mHasError )
        return QByteArray();

    if( mIsCurrentChunkDataRead )
        return mCurrentChunkData;

    // read chunk data
    mIsCurrentChunkDataRead = true;
    mCurrentChunkData.resize( mCurrentChunkHeader.mSize );
//TODO: handle reading of complete riff/list chunk
    // try to read
    const qint64 dataRead = mDevice->read( mCurrentChunkData.data(), mCurrentChunkHeader.mSize );
    if( dataRead == static_cast<qint64>(mCurrentChunkHeader.mSize) )
    {
        mCurrentChunkOffset += dataRead;

        // has pad byte?
        if( mCurrentChunkHeader.mSize & 1 )
        {
            // skip pad byte
            const qint64 absuluteOffset = mDevice->pos();
            if( ! mDevice->seek(absuluteOffset+1) )
            {
                mHasError = true;
                mErrorString = mDevice->errorString();
            }

            mCurrentChunkOffset += 1;
        }
    }
    else
    {
        mHasError = true;
        mErrorString = mDevice->errorString();
    }

    return mCurrentChunkData;
}

bool
RiffStreamReaderPrivate::openList()
{
// qDebug() << "opening list begin"<<mHasError<<mIsCurrentChunkFile<<mIsCurrentChunkList<<mIsCurrentChunkDataRead;

// TODO: handle read of complete riff/list chunk read before
    if( mHasError ||
        (! (mIsCurrentChunkFile || mIsCurrentChunkList)) )
        return false;

    const quint32 listEndOffset =
        mCurrentChunkOffset + mCurrentChunkHeader.mSize - sizeof(FourCharCode); // id is part of size
    mListStack.push( ListData(listEndOffset) );

// qDebug() << "opening list:" << mCurrentChunkHeader.mId.toString() << mCurrentChunkOffset << "..." << listEndOffset;

    // reset state
    mIsCurrentChunkDataRead = true;
    mCurrentChunkData.clear();
    mIsCurrentChunkList = false;
    mIsCurrentChunkFile = false;

    return true;
}

bool
RiffStreamReaderPrivate::closeList()
{
// qDebug() << "closing list begin";
    if( mHasError ||
        mListStack.isEmpty() )
        return false;

// qDebug() << "closing list, jump from"<< mCurrentChunkOffset<<"to"<<mListStack.top().mEndOffset;

    // remove list from stack and jump to end of list in stream
    mCurrentChunkOffset = mListStack.pop().mEndOffset;

    if( ! mDevice->seek(mDeviceStartOffset + mCurrentChunkOffset) )
    {
        mHasError = true;
        mErrorString = mDevice->errorString();
        return false;
    }

    // reset state
    mIsCurrentChunkDataRead = true;
    mCurrentChunkData.clear();
    mIsCurrentChunkList = false;
    mIsCurrentChunkFile = false;

    return true;
}

}
