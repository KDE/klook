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
#include <KIO/Job>
#include <KUrl>

class QTemporaryFile;
class FileTypeIdentifier;

class File : public QObject
{
    Q_OBJECT
    Q_ENUMS(FileType)
public:
    enum FileType
    {
        Undefined = 0,
        Image,
        Video,
        Txt,
        Directory,
        Audio,
        OkularFile,
        MimetypeFallback
    };
    File( QObject* parent );
    File( KUrl url = KUrl(), FileType type = Undefined, const QString& mime = "", QObject* parent = 0 );

public slots:
    QUrl url() const;
    void setUrl( QUrl url );

    File::FileType type();
    void setType(FileType type);

    QString mime();
    void setMime(const QString &mime);

    void load(); // download file and return path to temporary file
    bool isLoaded() const; // is file downloaded?

    QString tempFilePath() const;

private slots:
    void slotResult(KJob *job);

signals:
    void loaded();
    void dataChanged();
    void error(QString errorText);

private:
    void loadMimeAndType(bool force = false);

    KUrl m_url;
    mutable FileType m_type;
    mutable QString m_mime;
    QTemporaryFile *m_tempFile;
    bool m_isLoaded;
    FileTypeIdentifier *m_identifier;
};

class FileTypeIdentifier
{
public:
    FileTypeIdentifier();
    QPair<File::FileType, QString> getTypeAndMime(QUrl url) const;

private:
    File::FileType getType(const QString& mime, const QString& path) const;
    QString getMime(const QUrl &url) const;

    QList<QByteArray> supportedImageFormats;
    QStringList m_mimeTypes;
};

#endif // FILE_H
