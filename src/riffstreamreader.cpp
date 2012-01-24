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

#include "riffstreamreader.h"
#include "riffstreamreader_p.h"


namespace Koralle
{

RiffStreamReader::RiffStreamReader( QIODevice* device )
  : d( new RiffStreamReaderPrivate(device) )
{
}

RiffStreamReader::~RiffStreamReader()
{
    delete d;
}


bool
RiffStreamReader::atEnd() const
{
    return d->atEnd();
}

//     Error RiffStreamReader::error() const;
QString
RiffStreamReader::errorString() const
{
    return d->mErrorString;
}

bool
RiffStreamReader::hasError() const
{
    return d->mHasError;
}

FourCharCode
RiffStreamReader::chunkId() const
{
    return d->mCurrentChunkHeader.mId;
}

bool
RiffStreamReader::isListChunk() const
{
    return d->mIsCurrentChunkList;
}

bool
RiffStreamReader::isFileChunk() const
{
    return d->mIsCurrentChunkFile;
}

quint32
RiffStreamReader::chunkSize() const
{
    return d->mCurrentChunkHeader.mSize;
}

QByteArray
RiffStreamReader::chunkData() const
{
    return d->chunkData();
}

void
RiffStreamReader::setDevice( QIODevice* device )
{
    d->setDevice( device );
}

bool
RiffStreamReader::readNextChunkHeader()
{
    return d->readChunkHeader();
}

bool
RiffStreamReader::openList()
{
    return d->openList();
}

bool
RiffStreamReader::closeList()
{
    return d->closeList();
}

}
