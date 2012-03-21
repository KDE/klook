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


#include <QApplication>
#include <QDeclarativeContext>
#include <QLibraryInfo>
#include <QFileInfo>
#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QTranslator>

#include <QLibraryInfo>
#include <QDebug>

#include <qtsingleapplication/qtsingleapplication.h>

#include "declarativeviewer.h"
#include "video.h"
//#include "file.h"
#include "workerthread.h"

#include "previewgenerator.h"

#define QT_SINGLE_APP


int main(int argc, char *argv[])
{
    QApplication::setGraphicsSystem( "raster" );

    QtSingleApplication app( argc, argv );

    QString message;
    for ( int a = 1; a < argc; ++a )
    {
        message += QString::fromUtf8(argv[ a ]);
        if ( a < argc - 1 )
            message += ";";
    }
    //    qDebug() << message;

    if ( app.sendMessage( message ) )
    {
        qDebug("app is running...");

        return 0;
    }

    app.setApplicationName( "KLook" );

    QString locale = QLocale::system().name();
    QString libPath = QLibraryInfo::location( QLibraryInfo::LibrariesPath ) ;
    QTranslator translator;
    translator.load( QString( "klook_" ) + locale, libPath + "/klook/translations" );
    app.installTranslator( &translator );

    QStringList files = QApplication::arguments();
    files.removeFirst(); // remove application name

    DeclarativeViewer view( files );

    QFileInfo fi( QCoreApplication::applicationFilePath() );
    QString qmlPath = QLibraryInfo::location( QLibraryInfo::LibrariesPath ) + "/" + fi.baseName();

    view.setSource( QUrl::fromLocalFile( qmlPath + "/main.qml" ) );
//    view.setSource( QUrl::fromLocalFile( "/home/julia/work/klook/src/klook/qml/klook/main.qml" ) );

    QObject* rootObject = dynamic_cast<QObject*>( view.rootObject() );

    //QObject::connect( &view, SIGNAL( fileData( QVariant,QVariant ) ), rootObject, SLOT( updateData( QVariant, QVariant ) ) );
    QObject::connect( &view, SIGNAL( setFullScreenState() ), rootObject, SLOT( setFullScreenState() ) );
    QObject::connect( &view, SIGNAL( setEmbeddedState() ), rootObject, SLOT( setEmbeddedState() ) );
    QObject::connect( rootObject, SIGNAL( setGalleryView( bool ) ), &view, SLOT( onSetGallery( bool ) ) );

    //view.show();
    view.setAttribute( Qt::WA_QuitOnClose );

    QObject::connect( &app, SIGNAL( messageReceived( const QString& ) ), &view, SLOT( handleMessage( const QString& ) ) );

    app.setActivationWindow( &view, false );

    QObject::connect( &view, SIGNAL( needToShow() ), &app, SLOT( activateWindow() ) );

    //app.setQuitOnLastWindowClosed(true);

    return app.exec();
}
