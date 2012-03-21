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

#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include "file.h"

#include <QtCore/QThread>
#include <QtCore/QVariant>


//class QStringList;
#include <QStringList>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread( const QStringList& urls, QObject *parent = 0 );

protected:
    void run();

signals:
    void fileProcessed( const File *file );
    void fail();

public slots:
    void processUrl( const QStringList& urls );

private:
    QString getMime( const QString &st ) const;
    File::FileType getType( const QString& ) const;

    bool isFound; // found at least one supported file
    const QStringList& urls;
    QList<QByteArray> supportedImageFormats;
    QStringList m_mimeTypes;
};

#endif // WORKERTHREAD_H
