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
#include <QtCore/QThread>

#include "file.h"

class ListItem;
class DirectoryItem;
class QTimer;

class FileModel : public QAbstractListModel
{
  Q_OBJECT

public:
    explicit FileModel(QObject* parent = 0);

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    const QHash<int, QByteArray> & roleNames () const;
    void appendRow( ListItem* item );
    void refreshRow( const QModelIndex &index );
    QModelIndex indexFromRowNumber( int row );
    void reset();

    void refreshItem(ListItem *item);
    void setRoleNames(const QHash<int,QByteArray> &roleNames) { QAbstractItemModel::setRoleNames(roleNames); }

public slots:
    void append(File *file);
    void scanDirectory(int index);
    int count();
    File *file(int index);

private:
    ListItem* m_prototype;
    QList<ListItem*> m_list;
};

class ListItem: public QObject
{
    Q_OBJECT

public:
    enum
    {
        FilePathRole = Qt::UserRole + 1,
        TypeRole,
        LastModifiedRole,
        ContentSizeRole,
        CountRole,       // object count for folders
        MimeRole
    };

    ListItem(File *file, QObject *parent)
        : QObject(parent), m_file(file) {}
    virtual ~ListItem() {}

    static ListItem *newItem(File *file, QObject *parent);
    static QHash<int, QByteArray> roleNames();

    virtual QVariant data(int role) const;

    virtual QString path() const;
    void setPath(QString path) { m_file->setUrl(QUrl(path)); }

    virtual QString mime() const;

    virtual int type() const;
    void setType(int type) { m_file->setType(static_cast<File::FileType>(type)); }

    File *file() { return m_file; }

signals:
    void dataChanged();

private:
    File *m_file;
};

class DirectorySizeFinder : public QThread
{
public:
    DirectorySizeFinder( QString path, QObject* parent = 0 )
        : QThread( parent ), path( path ), m_size( 0 ), count( 0 ) {}
    virtual void run ();

    qint64 size() const { return m_size; }
    int fileCount() const { return count; }

private:
    QString path;
    qint64 m_size;
    int count;
};

class DirectoryItem : public ListItem
{
    Q_OBJECT
public:
    DirectoryItem(File *file, QObject* parent = 0);
    ~DirectoryItem();
    QVariant data(int role) const;
    void startScan();
private slots:
    void timeout();

private:
    QString formatSize(qint64 size);
    void notifyModel();

    bool isScanned;
    QDir dir;
    DirectorySizeFinder *sizeFinder;
    QTimer *timer;
    qint64 size;
    int count;
};

class UnsupportedItem : public ListItem
{
    Q_OBJECT
public:
    UnsupportedItem(File *file, QObject* parent = 0);
    QVariant data( int role ) const;
};

#endif // FILEMODEL_H
