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

#include "previewgenerator.h"

#include <QPainter>

#include <kfile.h>
#include <kicon.h>

#include "filemodel.h"
#include <QtCore/QDebug>

PreviewGenerator *PreviewGenerator::instance = 0;

#define NO_DIRECTORY_THUMBNAIL

PreviewGenerator::PreviewGenerator(QObject *parent)
    : QObject( parent )
    , m_model( 0 )
    , m_job( 0 )
{
    defaultPreview.load( ":images/pla-empty-box.png" );
    videoPixmap.load( ":images/play-empty.png" );
    m_plugins = KIO::PreviewJob::availablePlugins();

#ifdef NO_DIRECTORY_THUMBNAIL
    m_plugins.removeAll("directorythumbnail");
#endif

}

void PreviewGenerator::notifyModel( QUrl url )
{
    if ( m_model )
    {
        for (int i = 0; i < m_model->rowCount(); i++)
        {
            QModelIndex index = m_model->indexFromRowNumber( i );
            if (m_model->data( index, ListItem::FilePathRole).toUrl() == url)
                m_model->refreshRow( index );
        }
    }
}

void PreviewGenerator::setPreview( const KFileItem &item, const QPixmap &pixmap )
{
    QPixmap pict = pixmap;
    m_fileList.removeAll(item);

    if ( item.mimetype().startsWith("video/") )
    {
        QPainter p(&pict);
        QPixmap scaledPixmap = videoPixmap.scaled(pict.width() / 2, pict.height() / 2,  Qt::KeepAspectRatio, Qt::SmoothTransformation );
        p.drawPixmap( pict.width() / 2 - scaledPixmap.width() / 2, pict.height() / 2 - scaledPixmap.height() / 2 ,  scaledPixmap );
    }
    previews.insert(item.url(), pict);
    notifyModel(item.localPath());
}

void PreviewGenerator::deleteJob()
{
    delete m_job;
    m_job = 0;
}

QPixmap PreviewGenerator::getPreviewPixmap(QString filePath)
{
    if (previews.contains(filePath))
        return previews[filePath];

    return defaultPreview;
}

PreviewGenerator * PreviewGenerator::createInstance()
{
    if (!instance)
        instance = new PreviewGenerator;
    return instance;
}

void PreviewGenerator::setFiles( const QStringList &list )
{
    stop();
    m_fileList.clear();

    for ( int i = 0; i < list.size(); i++ )
    {
        QFileInfo fi(list[i]);
        KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, fi.absoluteFilePath(), false);
        m_fileList.append(fileItem);
    }
}

void PreviewGenerator::setModel( FileModel* model )
{
    m_model = model;
}

void PreviewGenerator::start()
{
    if (m_fileList.isEmpty())
        return;

    if(m_job)
        m_job->kill();

    m_job = KIO::filePreview( m_fileList, 1000, 0, 0, 0, true, false, &m_plugins );
    m_job->setIgnoreMaximumSize();
    m_job->setAutoDelete( false );

    connect(m_job, SIGNAL( gotPreview( const KFileItem&, const QPixmap& ) ), SLOT( setPreview( const KFileItem&, const QPixmap& ) ) );
    connect(m_job, SIGNAL( failed( KFileItem ) ), SLOT( previewFailed( KFileItem ) ) );
    connect(m_job, SIGNAL( result( KJob* ) ), SLOT( deleteJob() ) );
}

void PreviewGenerator::stop()
{
    if (m_job)
        m_job->suspend();
}

void PreviewGenerator::previewFailed( KFileItem item )
{
    if ( item.mimetype().startsWith( "text/" ) )
    {
        QStringList listP;
        listP << "textthumbnail";
        KFileItemList list;
        list << item;
        KIO::PreviewJob* job = KIO::filePreview( list, 1000, 0, 0, 0, true, true, &listP );
        job->setIgnoreMaximumSize();
        job->setAutoDelete( true );
        connect( job, SIGNAL( gotPreview( const KFileItem&, const QPixmap& ) ), SLOT( setPreview( const KFileItem&, const QPixmap& ) ) );
    }
    else
    {
        KIcon icon(item.iconName(), 0, item.overlays());
        QPixmap pixmap = icon.pixmap(500);
        setPreview(item, pixmap);
    }
}
