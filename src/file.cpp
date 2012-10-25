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

#include <KIO/JobClasses>
#include <KIO/NetAccess>
#include <Phonon/BackendCapabilities>
#include <KMimeType>
#include <QtCore/QTemporaryFile>
#include <QtCore/QMetaType>
#include <QtCore/QDir>
#include <QtGui/QImageReader>
#include <QtGui/QApplication>
#include <kuiserverjobtracker.h>

File::File(QObject* parent)
    : QObject(parent)
    , m_type(Progress)
    , m_tempFile(0)
    , m_isLoaded(0)
    , m_job(0)
{
}

File::File(KUrl url, QObject* parent)
    : QObject( parent )
    , m_url(url)
    , m_type(File::Progress)
    , m_tempFile(0)
    , m_isLoaded(0)
    , m_mimeJobStarted(false)
    , m_job(0)
    , m_downloadInProgress(false)
{
    m_isLoaded = m_url.isLocalFile();
}

File::~File()
{
    stopDownload();
}

KUrl File::url() const
{
    return m_url;
}

void File::setUrl(QUrl url)
{
    m_url = url;
    m_isLoaded = url.isLocalFile();
}

File::FileType File::type()
{
    return m_type;
}

void File::setType(FileType type)
{
    m_type = type;
}

QString File::mime()
{
    return m_mime;
}

void File::setMime(const QString &mime)
{
    m_mime = mime;
}

void File::load()
{
    if(type() == Progress) {
        if(!m_mimeJobStarted)
            loadType();
        else
            return;
    }
    else if(needDownload()) {
        download();
    }
}

void File::download()
{
    if(m_downloadInProgress) {
        qDebug() << "Trying to download file for the second time...";
        return;
    }

    if(!m_tempFile) {
        m_tempFile = new QTemporaryFile(this);
    }
    if (m_tempFile->open()) {
        m_downloadInProgress = true;
        m_job = KIO::file_copy(m_url, KUrl(m_tempFile->fileName()), -1, KIO::Overwrite | KIO::HideProgressInfo);
        m_job->setAutoDelete(false);
        connect(m_job, SIGNAL(result(KJob *)), SLOT(slotDownloadResult(KJob *)));
        KIO::getJobTracker()->registerJob( m_job );
    }
    emit dataChanged();
}

bool File:: needDownload()
{
    bool r = true;
    FileType t = type();
    if(t == File::Image || t == File::Txt) {
        r = true;
    }
    else if(t != File::Directory) {
        r = false;
    }
    return !url().isLocalFile() && r;
}

bool File::isLoaded() const
{
    return m_isLoaded;
}

bool File::downloadInProgress() const
{
    return m_downloadInProgress;
}

void File::stopDownload()
{
    if(m_job) {
        m_downloadInProgress = false;
        m_isLoaded = false;
        m_job->kill();
        KIO::getJobTracker()->unregisterJob(m_job);
        delete m_job;
        m_job = 0;
        delete m_tempFile;
        m_tempFile = 0;
    }
}

QString File::tempFilePath() const
{
    return m_tempFile ? m_tempFile->fileName() : QString();
}

QString File::error() const
{
    return m_error;
}

void File::slotDownloadResult(KJob *job)
{
    m_downloadInProgress = false;
    if (job->error()) {
        setType(Error);
        m_error = job->errorString();
    }
    else {
        m_isLoaded = true;
    }

    KIO::getJobTracker()->unregisterJob(job);
    job->deleteLater();
    m_job = 0;

    emit dataChanged();
}

void File::resultMimetypeJob(KJob *job)
{
    if (!job->error())
    {
        setMime(dynamic_cast<KIO::MimetypeJob *>(job)->mimetype());
        FileType t = getFileType(mime(), url().fileName());
        setType(t);

        // now that we know file type we can download file if necessary
        if(needDownload()) {
            download();
        }
    }

    else {
        setType(File::Error);
        m_error = job->errorString();
    }

    emit dataChanged();
}

void File::loadType()
{
    if(!m_mimeJobStarted) {
        KIO::MimetypeJob *job = KIO::mimetype(url(), KIO::HideProgressInfo);
        connect(job, SIGNAL(result(KJob *)), SLOT(resultMimetypeJob(KJob*)));
        m_mimeJobStarted = true;
    }
}

File::FileType getFileType(const QString& mime, const QString& name)
{
    static QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
    static QStringList m_mimeTypes = Phonon::BackendCapabilities::availableMimeTypes();

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
            if ( m_mimeTypes.contains( mime ) )
                type = File::Video;

            if ( type == File::Progress )
            {
                if ( right == QLatin1String( "3gpp" ) ||
                     right == QLatin1String( "mp4" ) ||
                     right == QLatin1String( "x-theora+ogg" ) )
                    type = File::Video;
            }
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
