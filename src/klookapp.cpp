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

#include "klookapp.h"

#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsObject>
#include <QtQuick/QQuickItem>
#include <QUrl>

#include <KCmdLineArgs>
#include <KIcon>
#include <KStandardDirs>
#include <KUrl>

#include "declarativeviewer.h"
#include "filemodel.h"


KLookApp::KLookApp(QCommandLineParser& parser, QObject* parent)
    : QObject(parent)
    , m_viewer(0)
    , parser(parser)
{
}

KLookApp::~KLookApp()
{
    delete m_viewer->m_fileModel;
}

bool KLookApp::isEmbeddedParam() const
{
    return parser.isSet("embedded");
}

QRect KLookApp::rectParam() const
{
    QRect rc;
    if (parser.isSet("x") && parser.isSet("y") &&
         parser.isSet("w") && parser.isSet("h")) {
        int x, y, w, h;
        x = parser.value("x").toInt();
        y = parser.value("y").toInt();
        w = parser.value("w").toInt();
        h = parser.value("h").toInt();
        rc = QRect(x, y, w, h);
    }
    return rc;
}

QStringList KLookApp::urlsParam(const QString& workingDirectory) const
{
    QStringList urls;
    urls.reserve(parser.positionalArguments().size());
    for (const auto& positionalArgument: parser.positionalArguments()) { //TODO: std::transform?
        urls << QUrl::fromUserInput(positionalArgument, workingDirectory).url();
    }
    return urls;
}

int KLookApp::handleCmdLIne(const QString& workingDirectory)
{
    QStringList urls = urlsParam(workingDirectory);
    QRect rc = rectParam();
    bool embedded = isEmbeddedParam();

    int index = parser.value("i").toInt();
    if (index >= urls.count()) {
        index = 0;
    }
    if(!m_viewer) {
        m_viewer = new DeclarativeViewer();
        QString qmlPath = KStandardDirs::locate("appdata", "main.qml");
        if (isLocal()) // this is hack for developers. should replace it with something better I guess
            qmlPath = "../src/qml/main.qml";

        if(!QFile::exists(qmlPath)) {
            qDebug() << "QML file not found";
            //exit(); FIXME
        }

        m_viewer->setSource(QUrl::fromLocalFile(qmlPath));

        QObject* rootObject = dynamic_cast<QObject*>(m_viewer->rootObject());

        QObject::connect(m_viewer, SIGNAL(setFullScreenState()), rootObject, SLOT(setFullScreenState()));
        QObject::connect(m_viewer, SIGNAL(setEmbeddedState()), rootObject, SLOT(setEmbeddedState()));
        QObject::connect(m_viewer, SIGNAL(setStartWindow()), rootObject, SLOT(setStartWindow()));
        QObject::connect(rootObject, SIGNAL(setGalleryView(bool)), m_viewer, SLOT(onSetGallery(bool)));
    }

    m_viewer->init(urls, embedded, rc, index);

    m_viewer->show();
    return 0;
}

bool KLookApp::isLocal() const
{
    return false; //FIXME
    /*const QString appPath = applicationFilePath();
    return !(appPath.startsWith(QLatin1String("/usr/bin")) || appPath.startsWith(QLatin1String("/usr/local/bin")));*/
}
