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

#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QDir>
#include <QDateTime>
#include <QThread>

#include <QDebug>
#include "file.h"
class ListItem;
class DirectoryItem;

class FileModel : public QAbstractListModel
{
  Q_OBJECT

public:
    explicit FileModel( ListItem* prototype, QObject* parent = 0 );
    ~FileModel();

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    const QHash<int, QByteArray> & roleNames () const;
    void appendRow( ListItem* item );
    void refreshRow( const QModelIndex &index );
    QModelIndex indexFromRowNumber( int row );
    void reset();

    void refreshItem(ListItem *item);

public slots:
    void append(QString path, File::FileType type, QString mime);
    void scanDirectory(int index);
    int count();

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

    ListItem(QString filePath, QString fileType, QString mime, QObject* parent = 0)
        : QObject( parent ), m_path( filePath ), m_type( fileType ), m_mime(mime) {}
    ListItem(QObject *parent = 0)
        : QObject(parent) {}
    virtual ~ListItem() {}

    QHash<int, QByteArray> roleNames() const;
public slots:
    bool loaded();
    void setLoaded( bool b );
    virtual QVariant data( int role ) const;
    virtual QString path() const;
    virtual QString type() const;
    virtual QString mime() const;

    void setPath(QString path) { m_path = path; }
    void setMimeType(QString type) { m_type = type; }

signals:
    void dataChanged();
    void imageChanged();

private:
    QString m_path;
    QString m_type;
    bool m_isLoaded;
    QString m_mime;
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

class QTimer;

class DirectoryItem : public ListItem
{
    Q_OBJECT
public:
    DirectoryItem( QString filePath, QString type, QObject* parent = 0 );
    ~DirectoryItem();
    QVariant data( int role ) const;
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
    UnsupportedItem(QString filePath, QString type, QString mime, QObject* parent = 0);
    QVariant data( int role ) const;
};

#endif // FILEMODEL_H
