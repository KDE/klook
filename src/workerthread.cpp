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

#include "workerthread.h"

#include <QMetaType>
#include <QDir>
#include <QImageReader>

#include <KDE/Phonon/BackendCapabilities>

#include <kmimetype.h>

WorkerThread::WorkerThread( const QStringList& urls, QObject *parent )
    : QThread( parent )
    , isFound( false )
    , urls( urls )
    , supportedImageFormats( QImageReader::supportedImageFormats() )
{
    m_mimeTypes = Phonon::BackendCapabilities::availableMimeTypes();
}

void WorkerThread::run()
{
    processUrl( urls );

    if ( !isFound )
        emit fail();
}


void WorkerThread::processUrl( const QStringList &urls )
{
    QString path;

    foreach ( const QString& str, urls )
    {
        if ( !QDir::isAbsolutePath( str ) )
            path = QDir::currentPath() + "/" + str;
        else
            path = str;

        if ( QFile::exists( path ) )
        {
            QString mime = getMime( str );
            File::FileType type = getType( mime, path );
            const File *file = new File( path, type, mime );
            emit fileProcessed(file);
            isFound = true;
        }
        else
            qWarning() << "File not found " << str;
    }
}

QString WorkerThread::getMime( const QString& st ) const
{
    KMimeType::Ptr ptr =  KMimeType::findByFileContent( st );
    KMimeType* type = ptr.data();
    return type->name();
}

File::FileType WorkerThread::getType( const QString& mime, const QString& path ) const
{
    // this method is a complete mess right now
    // information about supported types should not be hardcoded

    int delimiter = mime.indexOf( '/' );

    File::FileType type = File::Undefined;

    if ( delimiter != -1 )
    {
        QString left = mime.left( delimiter );

        if ( left == QLatin1String( "image" ) )
        {
            QString right = mime.mid(delimiter + 1);
            if ( supportedImageFormats.contains( right.toLatin1()) || right == "x-xpixmap" )
                type = File::Image;
            else if ( supportedImageFormats.contains( "svg" ) && ( right == ( "svg+xml" ) || right == ( "svg+xml-compressed" ) ) )
                type = File::Image;
        }
        else if ( left == QLatin1String( "video" ) )
        {
            QString right = mime.mid( delimiter + 1 );
#if 1
            if ( m_mimeTypes.contains( mime ) )
                type = File::Video;

            if ( type == File::Undefined )
            {
                if ( right == QLatin1String( "3gpp" ) ||
                     right == QLatin1String( "mp4" ) ||
                     right == QLatin1String( "x-theora+ogg" ) )
                    type = File::Video;
            }
#else
            if ( right == QLatin1String("x-msvideo") ||
                 right == QLatin1String("mp4") ||
                 right == QLatin1String("3gpp") ||
                 right == QLatin1String("x-flv") ||
                 right == QLatin1String("quicktime") )
                type = File::Video;
#endif
        }
        else if ( left == QLatin1String( "audio" ) )
        {
            QString right = mime.mid( delimiter + 1 );

            if ( m_mimeTypes.contains( mime ) )
                type = File::Audio;

            if ( type == File::Undefined )
            {
                if ( right == QLatin1String( "ogg" ) ||
                     right == QLatin1String( "mpeg" ) ||
                     right == QLatin1String( "x-speex+ogg" ) ||
                     right == QLatin1String( "x-vorbis+ogg" ) ||
                     right == QLatin1String( "x-flac+ogg" ) )
                    type = File::Audio;
            }
        }
        else if ( left == QLatin1String( "text" ) ||
                  mime == QLatin1String( "application/x-zerosize" ) ||
                  mime == QLatin1String( "application/x-shellscript" ) )
            type = File::Txt;
        else if( mime.mid(delimiter + 1) == QLatin1String("directory"))
        {
            type = File::Directory;
        }
    }

    if(mime == "application/pdf" || mime == "application/vnd.oasis.opendocument.text" ||
            mime == "application/x-vnd.oasis.opendocument.text" || mime == "image/x-djvu" ||
            mime == "application/x-chm" || mime == "application/x-fictionbook+xml")
    {
        type = File::OkularFile;
    }

    if ( type == File::Undefined )
    {
        if ( mime == QLatin1String( "application/x-matroska" ) ||
             mime == QLatin1String( "application/vnd.rn-realmedia" ) )
            type = File::Video;
        else if ( mime == QLatin1String( "bmp" ) )
            type = File::Image;
        else if ( mime == QLatin1String( "application/octet-stream" ) )
        {
            QFileInfo fi( path );
            if ( fi.suffix() == QLatin1String( "mp3" ) ||
                 fi.suffix() == QLatin1String( "ogg" ) )
                type = File::Audio;
        }
    }

    return type;
}
