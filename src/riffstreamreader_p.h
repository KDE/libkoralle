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

#ifndef KORALLE_RIFFSTREAMREADER_P_H
#define KORALLE_RIFFSTREAMREADER_P_H

// lib
#include "riffstreamreader.h"
#include "fourcharcode.h"
// Qt
#include <QtCore/QIODevice>
#include <QtCore/QStack>
#include <QtCore/QString>
#include <QtCore/QByteArray>


namespace Koralle
{

struct ChunkHeader
{
    ChunkHeader() : mSize( 0 ) {}

    char* data() { return reinterpret_cast<char*>(this); }
    void clear() { mId.mCode.asUint = 0; mSize = 0; }

public:
    FourCharCode mId;
    quint32 mSize;
};

struct ListData
{
    explicit ListData( quint32 endOffset ) : mEndOffset( endOffset) {}
    ListData() {}

public:
    // offset after end of list content
    quint32 mEndOffset;
};


// TODO: does not handle sequential device types yet, only random access ones
class RiffStreamReaderPrivate
{
public:
    explicit RiffStreamReaderPrivate( QIODevice* device );

public:
    bool atEnd() const;

    bool readChunkHeader();
    QByteArray chunkData();

    void setDevice( QIODevice* device );
    void resetState();
    bool openList();
    bool closeList();

public:
    QIODevice* mDevice;
    qint64 mDeviceStartOffset;

    ChunkHeader mCurrentChunkHeader;
    bool mIsCurrentChunkList : 1;
    bool mIsCurrentChunkFile : 1;
    bool mIsCurrentChunkDataRead : 1;
    QByteArray mCurrentChunkData;
    qint64 mCurrentChunkOffset;

    QStack<ListData> mListStack;

    bool mHasError;
    QString mErrorString;
};


inline bool
RiffStreamReaderPrivate::atEnd() const
{
    return mHasError ||
           mDevice ? mDevice->atEnd() : true;
}

}

#endif