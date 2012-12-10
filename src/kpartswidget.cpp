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

#include "kpartswidget.h"

#include <QtGui/QDesktopWidget>

#include <KToolBar>

KPartsWidget::KPartsWidget(QWidget *parent)
    : KParts::MainWindow(parent, static_cast<Qt::WindowFlags>(KDE_DEFAULT_WINDOWFLAGS))
{
    m_service = KService::serviceByDesktopPath("okular_part.desktop");

    // because we don't have KMainWindow in KLook
    // but use it for Okular integration klook tries
    // to exit when okular's window is deleted
    KGlobal::setAllowQuit(false);

    if(m_service) {
        m_part = m_service->createInstance<KParts::ReadOnlyPart>(0, QVariantList() << "Print/Preview");
        if(m_part) {
            setCentralWidget(m_part->widget());
            // we don't need toolabrs being shown
            foreach(KToolBar *toolbar, toolBars())
                toolbar->hide();
        }
    }
}

void KPartsWidget::setUrl(const QString &url)
{
    if(m_part) {
        m_part->openUrl(url);
    }
}

QStringList KPartsWidget::supportedMimeTypes() const
{
    return QStringList();
}

QSize getDocumentWindowSize()
{
    QDesktopWidget dw;
    QSize size;

    if (dw.height() < dw.width()) {
        size.setHeight(dw.height() * 0.8);
        size.setWidth(size.height () / 1.4142 + 40  * 1.4142);
    }
    else {
        size.setWidth(dw.width() * 0.8);
        size.setHeight(size.width() * 1.4142);
        size.setWidth(size.width() + 40); // caption
    }
    return size;
}
