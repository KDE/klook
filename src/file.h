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

#include <QtCore/QObject>

#include <KUrl>

namespace KIO {
class FileCopyJob;
}

class QTemporaryFile;
class FileTypeIdentifier;
class KJob;

class File : public QObject
{
    Q_OBJECT
    Q_ENUMS(FileType)
public:
    enum FileType
    {
        Progress = 0,
        Image,
        Video,
        Txt,
        Directory,
        Audio,
        OkularFile,
        MimetypeFallback,
        Error
    };
    File(QObject* parent);
    File(KUrl url = KUrl(), QObject* parent = 0);
    ~File();

    KUrl url() const;
    void setUrl(QUrl url);

    File::FileType type() const;
    void setType(FileType type);

    void setCurrent(bool isCurrent)
    {
        m_isCurrent = isCurrent;
    }

    QString mime() const;
    void setMime(const QString &mime);

    void load();
    bool isLoaded() const; // is file downloaded?

    bool downloadInProgress() const;

    void stopDownload();
    QString tempFilePath() const;

    QString error() const;

public slots:
    void slotDownloadResult(KJob *job);
    void resultMimetypeJob(KJob *job);

signals:
    void dataChanged();

private:
    void loadType();
    void download(); // download file and return path to temporary file
    bool needDownload();

    KUrl m_url;
    KIO::FileCopyJob *m_job;
    QString m_error;
    FileType m_type;
    QString m_mime;
    QTemporaryFile *m_tempFile;
    bool m_isLoaded;
    bool m_mimeJobStarted;
    bool m_downloadInProgress;
    bool m_isCurrent;
    bool m_checkedForExistance;
    bool m_fileExists;
};

File::FileType getFileType(const QString& mime, const QString& name);

#endif // FILE_H
