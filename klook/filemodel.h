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

class ListItem;

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

public slots:
    void append(QVariant path, QVariant type);

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
        MimeTypeRole
    };

    ListItem(QString filePath, QString fileType, QObject* parent = 0)
        : QObject( parent ), m_path( filePath ), m_type( fileType ) {}
    ListItem(QObject *parent = 0)
        : QObject(parent) {}
    virtual ~ListItem() {}

    QHash<int, QByteArray> roleNames() const;
public slots:
    bool loaded();
    void setLoaded( bool b );
    virtual QVariant data( int role );
    virtual QString path() const;
    virtual QString type() const;

    void setPath(QString path) { m_path = path; }
    void setType(QString type) { m_type = type; }

signals:
    void dataChanged();
    void imageChanged();

private:
    QString m_path;
    QString m_type;
    bool m_isLoaded;
};
#endif // FILEMODEL_H
