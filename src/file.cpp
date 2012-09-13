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
#include "progressdeclarativeitem.h"

#include <KIO/JobClasses>
#include <KIO/NetAccess>
#include <KIO/JobUiDelegate>
#include <KDE/Phonon/BackendCapabilities>
#include <KMimeType>
#include <QtCore/QTemporaryFile>
#include <QtCore/QMetaType>
#include <QtCore/QDir>
#include <QtGui/QImageReader>
#include <QtGui/QApplication>

File::File(QObject* parent)
    : QObject(parent)
    , m_type(Undefined)
    , m_tempFile(0)
    , m_isLoaded(0)
    , m_identifier(new FileTypeIdentifier)
{
}

File::File(KUrl url, FileType type, const QString& mime,QObject* parent)
    : QObject( parent )
    , m_url(url)
    , m_type(File::Undefined)
    , m_mime(mime)
    , m_tempFile(0)
    , m_isLoaded(0)
    , m_identifier(new FileTypeIdentifier)
    , m_jobStarted(false)
    , m_downloadInProgress(false)
{

}

KUrl File::url() const
{
    return m_url;
}

void File::setUrl(QUrl url)
{
    m_url = url;
}

File::FileType File::type()
{
    loadMimeAndType();
    return m_type;
}

void File::setType(FileType type)
{
    m_type = type;
}

QString File::mime()
{
    loadMimeAndType();
    return m_mime;
}

void File::setMime(const QString &mime)
{
    m_mime = mime;
}

void File::load()
{
    m_downloadInProgress = true;
    if(!m_tempFile)
    {
        m_tempFile = new QTemporaryFile(this);
        if (m_tempFile->open())
        {
            KIO::Job* job = KIO::file_copy(m_url, KUrl(m_tempFile->fileName()), -1, KIO::Overwrite | KIO::HideProgressInfo);
            connect(job, SIGNAL(result(KJob *)), SLOT(slotDownloadResult(KJob*)));
            ProgressDeclarativeItem::setJob(job);
        }
    }
}

bool File::isLoaded() const
{
    return m_isLoaded;
}

bool File::downloadInProgress() const
{
    return m_downloadInProgress;
}

QString File::tempFilePath() const
{
    return m_tempFile ? m_tempFile->fileName() : QString();
}

void File::slotDownloadResult(KJob *job)
{
    m_downloadInProgress = false;
    if(job->error())
        emit error(job->errorText());
    else
    {
        m_isLoaded = true;
        loadMimeAndType(true);
        emit dataChanged();
    }
}

void File::slotResult(KJob *job)
{
    if(!job->error())
    {
        setMime(dynamic_cast<KIO::MimetypeJob *>(job)->mimetype());
        FileType t = m_identifier->getType(mime(), url().fileName());
        setType(t);

        // check if we should download file. it should be done only for
        // some files we support well

        if(t == File::Audio || t == File::Image
                || t == File::Video || t == File::Txt) {
            load();
        }
        else if(t != File::Directory){
            t = File::MimetypeFallback;
        }

        emit dataChanged();
    }
}

void File::loadMimeAndType(bool force)
{
    if(!m_jobStarted) {
        KIO::MimetypeJob *job = KIO::mimetype(url(), KIO::HideProgressInfo);
        connect(job, SIGNAL(result(KJob *)), SLOT(slotResult(KJob*)));
        m_jobStarted = true;
    }
}

FileTypeIdentifier::FileTypeIdentifier()
    : supportedImageFormats(QImageReader::supportedImageFormats())
    , m_mimeTypes(Phonon::BackendCapabilities::availableMimeTypes())

{

}

QPair<File::FileType, QString> FileTypeIdentifier::getTypeAndMime(QUrl url) const
{
    QString mime = getMime(url);
    File::FileType t = getType(mime, url.toLocalFile());
    return qMakePair(t, mime);
}

QString FileTypeIdentifier::getMime(const QUrl &url) const
{
    return "bla";
}

File::FileType FileTypeIdentifier::getType(const QString& mime, const QString& name) const
{
    // this method is a complete mess right now
    // information about supported types should not be hardcoded
    int delimiter = mime.indexOf( '/' );

    File::FileType type = File::MimetypeFallback;

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

            if ( type == File::MimetypeFallback)
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
            mime == "application/x-vnd.oasis.opendocument.text" || /*mime == "image/x-djvu" || mime == "image/vnd.djvu" ||*/
            mime == "application/x-chm" || mime == "application/x-fictionbook+xml" || mime == "application/xhtml+xml")
    {
        type = File::OkularFile;
    }

    if ( type == File::MimetypeFallback)
    {
        if ( mime == QLatin1String( "application/x-matroska" ) ||
             mime == QLatin1String( "application/vnd.rn-realmedia" ) )
            type = File::Video;
        else if ( mime == QLatin1String( "bmp" ) )
            type = File::Image;
        else if ( mime == QLatin1String( "application/octet-stream" ) )
        {
            QFileInfo fi( name );
            if ( fi.suffix() == QLatin1String( "mp3" ) ||
                 fi.suffix() == QLatin1String( "ogg" ) )
                type = File::Audio;
        }
    }
    return type;
}
