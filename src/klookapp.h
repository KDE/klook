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

#ifndef KLOOK_APP_H
#define KLOOK_APP_H

#include <QObject>
#include <QString>

class DeclarativeViewer;
class QCommandLineParser;

class KLookApp : public QObject
{
    Q_OBJECT

public:
    KLookApp(QCommandLineParser& parser, QObject* parent = nullptr);
    ~KLookApp();
    int handleCmdLIne(const QString& workingDirectory);

private:
    bool isLocal() const;
    bool isEmbeddedParam() const;

    QStringList urlsParam(const QString& workingDirectory) const;
    QRect rectParam() const;

    DeclarativeViewer* m_viewer;

    QCommandLineParser& parser;
};

#endif // KLOOK_APP_H
