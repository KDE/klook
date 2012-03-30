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

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#include "klookapp.h"

static const char description[] = I18N_NOOP( "KLook is a quick preview future" );
static const char version[] = "0.1";

int main(int argc, char *argv[])
{
    QApplication::setGraphicsSystem( "raster" );

    KAboutData about( "KLook", 0, ki18n( "KLook" ),
                      version, ki18n( description ), KAboutData::License_GPL_V3,
                      ki18n( "(c) ROSA 2011-2012" ) );

    about.addAuthor( ki18n( "Julia Mineeva" ),
                     ki18n( "Developer" ),
                     "julia.mineeva@osinit.ru" );

    about.addAuthor( ki18n( "Evgeniy Auzhin" ),
                     ki18n( "Developer" ),
                     "evgeniy.augin@osinit.ru" );

    about.addAuthor( ki18n( "Sergey Borovkov" ),
                     ki18n( "Developer" ),
                     "sergey.borovkov@osinit.ru" );

    KCmdLineArgs::init( argc, argv, &about );

    KCmdLineOptions options;
    options.add( "+file", ki18n("A required argument 'file'" ) );
    options.add( "embedded", ki18n("turn on embedded mode" ) );
    options.add( "c", ki18n("coordinates icon" ) );
    KCmdLineArgs::addCmdLineOptions( options );

    if ( !KUniqueApplication::start() )
    {
        fprintf( stderr, "KLook is already running!\n" );
        return 0;
    }

    QStringList args;
    for ( int a = 1; a < argc; ++a )
        args << QString::fromUtf8( argv[ a ] );

    KLookApp a( args );
    return a.exec();
}
