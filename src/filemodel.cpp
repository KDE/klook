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

#include "filemodel.h"
#include "file.h"
#include "listitem.h"

#include <KGlobal>
#include <KLocale>

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentLoadingFile(0)
{
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_list.size()) {
        return QVariant();
    }
    return m_list.at(index.row())->data(role);
}

bool FileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (role == ListItem::FilePathRole) {
        m_list[ index.row() ]->setPath(value.toString());
    }
    else if (role == ListItem::TypeRole) {
        m_list[ index.row() ]->setType(value.toInt());
    }
    emit dataChanged(index, index);

    return true;
}

void FileModel::appendRow(ListItem *item)
{
    appendRows(QList<ListItem *>() << item);
}

void FileModel::appendRows(QList<ListItem *> items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach(ListItem *item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.append(item);
    }
    endInsertRows();

}

void FileModel::refreshRow(int row)
{
    emit dataChanged(index(row), index(row));
}

QModelIndex FileModel::indexFromRowNumber(int row)
{
    return index(row);
}

void FileModel::reset()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_list.clear();
    endRemoveRows();
}

int FileModel::rowFromFile(const File *file) const
{
    for(int i = 0; i < m_list.size(); i++) {
        if(m_list[i]->file() == file) {
            return i;
        }
    }

    return -1;
}

void FileModel::previewReady(const QString &path)
{
    emit gotPreview(path);
}

void FileModel::load(int row)
{
    if(row >= rowCount() || row < 0) {
        return;
    }

    // stop previous download
    if(m_currentLoadingFile) {
        m_currentLoadingFile->setCurrent(false);
        // this is redundant. need to remove later
        if(m_currentLoadingFile->downloadInProgress()) {
            m_currentLoadingFile->stopDownload();
        }
    }

    File *f = file(row);
    f->setCurrent(true);
    f->load();
    m_currentLoadingFile = f;
}

int FileModel::count() const
{
    return rowCount();
}

QUrl FileModel::url(int index) const
{
    return file(index)->url();
}

QString FileModel::fileName(int index) const
{
    return file(index)->url().fileName();
}

File *FileModel::file(int index) const
{
    return m_list[index]->file();
}

void FileModel::handleItemChange()
{
    ListItem* item = qobject_cast<ListItem *>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid()) {
        emit dataChanged(index, index);
    }
}

QModelIndex FileModel::indexFromItem(ListItem *item)
{
    const int i = m_list.indexOf(item);
    return i != -1 ? index(i) : QModelIndex();
}
