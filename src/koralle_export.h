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

#ifndef KORALLE_EXPORT_H
#define KORALLE_EXPORT_H

// Qt
#include <QtCore/QtGlobal>


#ifndef KORALLE_EXPORT
  // building the library?
# if defined(MAKE_KORALLE_LIB)
#  define KORALLE_EXPORT Q_DECL_EXPORT
  // using the library
# else
#  define KORALLE_EXPORT Q_DECL_IMPORT
# endif
#endif

#ifndef KORALLE_EXPORT_DEPRECATED
#  define KORALLE_EXPORT_DEPRECATED Q_DECL_DEPRECATED KORALLE_EXPORT
#endif

#endif
