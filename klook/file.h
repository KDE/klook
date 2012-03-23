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

#ifndef FILE_H
#define FILE_H

#include <QObject>

class File : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString name READ name WRITE setName )
    Q_PROPERTY( FileType type READ type WRITE setType )

public:
    enum FileType
    {
        Undefined = 0,
        Image,
        Video,
        Txt
    };

    File( QObject* parent = 0 );
    File( const QString& name = "", FileType type = Undefined, const QString& mime = "", QObject* parent = 0 );

    QString name() const;
    void setName( const QString& );

    File::FileType type() const;
    void setType( FileType );

    QString mime() const;
    void setMime( const QString& );

    static QString fileTypeToString( FileType );

private:
    QString     m_name;
    FileType    m_type;
    QString     m_mime;

};

#endif // FILE_H
