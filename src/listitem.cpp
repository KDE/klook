/* KLook
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors: Sergey Borovkov.
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

#include "listitem.h"
#include "listitemcontent.h"
#include "filemodel.h"

ListItem::ListItem(File *file, QObject *parent)
    : QObject(parent), m_file(file), m_content(0)
{
    m_file->setParent(this);
    connect(file, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
}

QString ListItem::path() const
{
    QString result;
    if(!m_file->url().isLocalFile() && m_file->isLoaded()) {
        result = m_file->tempFilePath();
    }
    else {
        result = m_file->url().pathOrUrl();
    }
    return result;
}

QVariant ListItem::data(int role) const
{
    switch (role) {
    case FilePathRole:
        return path();

    case TypeRole:
        return type();

    case MimeRole:
        return mime();

    case LocalFileRole:
        return m_file->url().isLocalFile();

    case FileNameRole:
        return m_file->url().fileName();

    case DownloadInProgress:
        return m_file->downloadInProgress();

    case UrlRole:
        return m_file->url().url();

    case LoadedRole:
        return m_file->isLoaded();

    case ErrorRole:
        return m_file->error();

    default:
        break;
    }

    // instantiate content only when roles really require it
    if(!m_content) {
        const int type = m_file->type();
        if(m_file->type() == File::Progress) {
            return QVariant();
        }

        QObject *parent = const_cast<QObject *>(qobject_cast<const QObject *>(this));
        if(type == File::Directory) {
            m_content = new ListItemDirectoryContent(m_file, parent);
        }
        else {
            m_content = new ListItemFallbackContent(m_file, parent);
        }
        connect(m_content, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
    }

    return m_content->data(role);
}

QHash<int, QByteArray> ListItem::roleNames()
{
    QHash<int, QByteArray> names;
    names[ErrorRole] = "error";
    names[FilePathRole] = "filePath";
    names[FileNameRole] = "fileName";
    names[UrlRole] = "fileUrl";
    names[TypeRole] = "type";
    names[MimeRole] = "mime";
    names[LoadedRole] = "isLoaded";
    names[LocalFileRole] = "isLocal";
    names[DownloadInProgress] = "downloadInProgress";

    // dirs and fallback
    names[StatCompleteRole] = "statComplete";
    names[DirSizeCompleteRole] = "dirSizeComplete";
    names[LastModifiedRole] = "lastModified";
    names[ContentSizeRole] = "contentSize";
    names[CountRole] = "countElements";

    return names;
}
