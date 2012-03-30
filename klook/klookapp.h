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

#include <kuniqueapplication.h>

class DeclarativeViewer;

class KLookApp : public KUniqueApplication
{
    Q_OBJECT

public:
    KLookApp( const QStringList& args );
    ~KLookApp();

    virtual int newInstance();

private:
    bool isLocal() const;
    
    DeclarativeViewer* m_viewer;

};

#endif // KLOOK_APP_H
