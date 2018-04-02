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

#include <stdio.h>

#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KLocale>
#include <KDBusService>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDir>
#include <QApplication>
#include <QString>
#include <QStringList>

#include "klookapp.h"

static const char description[] = I18N_NOOP("KLook is a quick file content preview program");
static const char version[] = "1.1";

int main(int argc, char *argv[])
{
    QApplication app {argc, argv};
    KAboutData aboutData("klook", i18n("KLook"),
                      version, i18n(description),  KAboutLicense::GPL_V3,
                      i18n("(c) ROSA 2011-2012"));

    aboutData.addAuthor(i18n("Julia Mineeva"),
                     i18n("Developer"),
                     "julia.mineeva@osinit.ru");

    aboutData.addAuthor(i18n("Evgeniy Auzhin"),
                     i18n("Developer"),
                     "evgeniy.augin@osinit.ru");

    aboutData.addAuthor(i18n("Sergey Borovkov"),
                     i18n("Developer"),
                     "sergey.borovkov@osinit.ru");

    aboutData.setProgramIconName("klook");

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("i"), i18n("File index "), QLatin1String("i"), QLatin1String("0")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("embedded"), i18n("Turn on embedded mode"), QLatin1String("0")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("x"), i18n("X position of the icon"), QLatin1String("x"), QLatin1String("0")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("y"), i18n("Y position of the icon"), QLatin1String("y"), QLatin1String("0")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("w"), i18n("Width of the icon"), QLatin1String("width"), QLatin1String("0")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("h"), i18n("Height of the icon"), QLatin1String("height"), QLatin1String("0")));
    parser.addPositionalArgument(QLatin1String("file"), i18n("A required argument 'file'"), QLatin1String(0));

    parser.process(app); // PORTING SCRIPT: move this to after any parser.addOption
    aboutData.processCommandLine(&parser);

    KLookApp a {parser};
    KDBusService service(KDBusService::Unique);

    QObject::connect(&service, &KDBusService::activateRequested, &a, [&parser, &a](const QStringList& arguments, const QString& workingDirectory){
        if (!arguments.isEmpty()) {
            parser.parse(arguments);
        }
        a.handleCmdLIne(workingDirectory);
    });
    a.handleCmdLIne(QDir::currentPath());

    return app.exec();
}
