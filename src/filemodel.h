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

#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QDateTime>

class File;
class ListItem;
class DirectoryItem;
class QTimer;

class FileModel : public QAbstractListModel
{
  Q_OBJECT

public:
    explicit FileModel(QObject* parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    const QHash<int, QByteArray> & roleNames () const;
    void appendRow(ListItem* item);
    void appendRows(QList<ListItem *> items);
    void refreshRow(int row);
    QModelIndex indexFromRowNumber(int row);
    void reset();

    void setRoleNames(const QHash<int,QByteArray> &roleNames) { QAbstractItemModel::setRoleNames(roleNames); }

    int rowFromFile(const File *file) const;

    void previewReady(const QString &path);

    Q_INVOKABLE void load(int row);
    Q_INVOKABLE File *file(int index);
    Q_INVOKABLE int count() const;

private slots:
    void handleItemChange();

signals:
    void gotPreview(QString path);

private:
    QModelIndex indexFromItem(ListItem *item);
    ListItem* m_prototype;
    QList<ListItem*> m_list;
    File *m_currentLoadingFile;
};

#endif // FILEMODEL_H
