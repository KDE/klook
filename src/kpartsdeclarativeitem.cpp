/* KLook
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors: Sergey Borovkov.
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

#include "kpartsdeclarativeitem.h"
#include "kpartswidget.h"

KPartsDeclarativeItem::KPartsDeclarativeItem(QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    QGraphicsProxyWidget(parent, wFlags), m_partWidget(new KPartsWidget())
{
    setWidget(m_partWidget);
}

QString KPartsDeclarativeItem::url() const
{
    return m_partWidget->url();
}

void KPartsDeclarativeItem::setUrl(QString url)
{
    m_partWidget->setUrl(url);
    emit urlChanged();
}
