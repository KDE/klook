/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "file.h"

File::File( QObject* parent )
    : QObject( parent )
    , m_name( "name" )
    , m_type( Undefined )
    , m_mime( "" )
{
}

File::File( const QString& name, FileType type, const QString& mime,QObject* parent )
    : QObject( parent )
    , m_name( name )
    , m_type( type )
    , m_mime( mime )
{
}

QString File::name() const
{
    return m_name;
}

void File::setName( const QString & name )
{
    m_name = name;
}

File::FileType File::type() const
{
    return m_type;
}

void File::setType( FileType type )
{
    m_type = type;
}

QString File::mime( ) const
{
    return m_mime;
}

void File::setMime( const QString & mime )
{
    m_mime = mime;
}

QString File::fileTypeToString( FileType ft )
{
    QString str;

    switch(ft)
    {
    case Image:
        str = "ImageType";
        break;
    case Video:
        str = "VideoType";
        break;
    case Txt:
        str = "TxtType";
        break;
    case Directory:
        str = "DirectoryType";
        break;
    default:
        str = "UndefinedType";
        break;
    }

    return str;
}
