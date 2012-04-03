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

#include <QStack>
#include <QTimer>
#include <QMessageBox>

#include <kglobal.h>
#include <klocale.h>

FileModel::FileModel( ListItem *prototype, QObject *parent )
    : QAbstractListModel( parent )
{
    setRoleNames( prototype->roleNames() ) ;
}

FileModel::~FileModel()
{
}


int FileModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_list.count();
}

QVariant FileModel::data( const QModelIndex &index, int role ) const
{
    if ( index.row() < 0 || index.row() >= m_list.size() )
        return QVariant();
    return m_list.at(index.row())->data(role);
}

bool FileModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( !index.isValid() )
        return false;

    if ( role == ListItem::FilePathRole )
        m_list[ index.row() ]->setPath( value.toString() );
    else if ( role == ListItem::MimeTypeRole )
        m_list[ index.row() ]->setMimeType( value.toString() );

    emit dataChanged( index, index );

    return true;
}

void FileModel::appendRow( ListItem *item )
{
    beginInsertRows( QModelIndex(), rowCount(), rowCount() );
    m_list.append( item );
    endInsertRows();
}

void FileModel::refreshRow( const QModelIndex & index )
{
    emit dataChanged( index, index );
}

QModelIndex FileModel::indexFromRowNumber( int row )
{
    return index( row );
}

void FileModel::reset()
{
    beginRemoveRows( QModelIndex(), 0, rowCount() - 1 );
    m_list.clear();
    endRemoveRows();
}

void FileModel::refreshItem(ListItem *item)
{
    for(int i = 0; i < m_list.size(); i++)
        if(m_list[i] == item)
        {
            refreshRow(index(i));
            break;
        }
}

void FileModel::append( QVariant path, QVariant type )
{
    if ( type == File::Directory )
    {
        appendRow(new DirectoryItem(path.toString(), type.toString(), this));
    }
    else
        appendRow( new ListItem(path.toString(), type.toString(), this ) );
}

void FileModel::scanDirectory(int index)
{
    if(index >= 0 && index < rowCount())
    {
        DirectoryItem *item = static_cast<DirectoryItem *>(m_list[index]);
        if(item)
            item->startScan();
    }
}

QString ListItem::path() const
{
    return m_path;
}

QString ListItem::type() const
{
    return m_mimeType;
}

QVariant ListItem::data( int role )
{
    switch ( role )
    {
    case FilePathRole:
        return path();
        break;

    case MimeTypeRole:
        return type();
        break;

    default:
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> ListItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ FilePathRole ] = "filePath";
    names[ MimeTypeRole ] = "mimeType";
    names[ LastModifiedRole ] = "lastModified";
    names[ ContentSizeRole ] = "contentSize";
    names[ CountRole ] = "countElements";

    return names;
}

bool ListItem::loaded()
{
    return m_isLoaded;
}

void ListItem::setLoaded( bool b )
{
    m_isLoaded = b;
    emit imageChanged();
}

DirectoryItem::DirectoryItem(QString filePath, QString type, QObject *parent)
    : ListItem(filePath, type, parent),
      isScanned( false ),
      dir( filePath ),
      sizeFinder( new DirectorySizeFinder( filePath ) ),
      timer( new QTimer( this ) ),
      size( 0 ),
      count( 0 )
{
}

QVariant DirectoryItem::data( int role )
{
    if(role == LastModifiedRole)
    {
        QFileInfo fi( path() );
        return KGlobal::locale()->formatDate( fi.lastModified().date() );
    }
    else if ( ( role == ContentSizeRole ) ||
              ( role == CountRole ) )
    {
        if ( role == ContentSizeRole )
            return KGlobal::locale()->formatByteSize( size );
        else // CountRole
            return QString::number( count );
    }
    else
        return ListItem::data(role);
}

void DirectoryItem::timeout()
{
    if ( sizeFinder->isFinished() )
        timer->stop();

    size = sizeFinder->size();
    count = sizeFinder->fileCount();

    notifyModel();
}

void DirectoryItem::notifyModel()
{
    FileModel *model;
    if((model = dynamic_cast<FileModel *>(parent())))
    {
        model->refreshItem(this);
    }
}

void DirectoryItem::startScan()
{
    // if it's not first time let's return
    if(isScanned)        
        return;

    isScanned = true;
    sizeFinder->start(QThread::LowPriority);
    connect(timer, SIGNAL(timeout()), SLOT(timeout()));
    timer->start(100);
}


void DirectorySizeFinder::run()
{
    QStack<QString> scan;
    scan.push(path);
    while(!scan.empty())
    {
        const QString topItem = scan.pop();
        QDir dir(topItem);
        QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);

        count += list.size();
        for(int i = 0; i < list.size(); i++)
        {            
            if(list[i].isDir())
                scan.push(list[i].absoluteFilePath());
            else
              m_size += list[i].size();
        }
    }
}
