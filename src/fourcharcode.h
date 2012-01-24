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

#ifndef KORALLE_FOURCHARCODE_H
#define KORALLE_FOURCHARCODE_H

// lib
#include "koralle_export.h"
// Qt
#include <QtCore/QString>


namespace Koralle
{

struct KORALLE_EXPORT FourCharCode
{
    FourCharCode() {}

    // TODO: find how to make the compiler turn this into an integer assignement
    FourCharCode(char c0, char c1, char c2, char c3)
    {
        mCode.asChars[0] = c0;
        mCode.asChars[1] = c1;
        mCode.asChars[2] = c2;
        mCode.asChars[3] = c3;
    }

    bool operator==(const FourCharCode& other) const
    { return (mCode.asUint == other.mCode.asUint); }

    QString toString() const { return QString::fromLatin1(mCode.asChars,4); }

    char* data() { return reinterpret_cast<char*>(&mCode); }

    union Code
    {
        quint32 asUint;
        char asChars[4];
        Code() : asUint( 0 ) {}
    } mCode;
};

}

#endif
