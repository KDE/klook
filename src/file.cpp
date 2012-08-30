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

#include <QtCore/QTemporaryFile>
#include <KIO/JobClasses>
#include <KIO/NetAccess>
#include <KIO/JobUiDelegate>

#include "progressdeclarativeitem.h"

File::File(QObject* parent)
    : QObject(parent)
    , m_type(Undefined)
    , m_tempFile(0)
{
}

File::File(KUrl url, FileType type, const QString& mime,QObject* parent)
    : QObject( parent )
    , m_url(url)
    , m_type(type)
    , m_mime(mime)
    , m_tempFile(0)
    , m_isLoaded(0)

{
}

QUrl File::url() const
{
    return m_url;
}

void File::setUrl(QUrl url)
{
    m_url = url;
}

File::FileType File::type() const
{
    return m_type;
}

void File::setType(FileType type)
{
    m_type = type;
}

QString File::mime() const
{
    return m_mime;
}

void File::setMime(const QString &mime)
{
    m_mime = mime;
}

void File::load()
{
    if(!m_tempFile)
    {
        m_tempFile = new QTemporaryFile(this);
        if (m_tempFile->open())
        {
            KIO::Job* job = KIO::file_copy(m_url, KUrl(m_tempFile->fileName()), -1, KIO::Overwrite | KIO::HideProgressInfo);
            connect(job, SIGNAL(result(KJob *)), SLOT(slotResult(KJob *)));
            ProgressDeclarativeItem::setJob(job);
        }
    }
}

bool File::isLoaded() const
{
    return m_isLoaded;
}

QString File::tempFilePath() const
{
    return m_tempFile ? m_tempFile->fileName() : QString();
}

void File::slotResult(KJob *job)
{
    if(job->error())
        emit error(job->errorText());
    else
    {
        m_isLoaded = true;
        emit loaded();
    }
}
