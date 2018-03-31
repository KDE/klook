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

#include "text.h"

#include <QtCore/QFile>
#include <QtCore/QTextCodec>

#include <KEncodingProber>
#include <KImageCache>

MyText::MyText(QObject* parent)
    : QObject(parent)
    , m_isPreview(false)
{
}

MyText::~MyText()
{
}

QString MyText::source() const
{
    return m_source;
}

void MyText::setSource(const QString& source)
{
    if (m_source == source) {
        return;
    }
    m_source = source;

    QFile f(m_source);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QByteArray data = f.readAll().data();
    KEncodingProber prober(KEncodingProber::Universal);
    prober.feed(data);

    if (prober.confidence() > 0.7) {
        m_sourceContent = QTextCodec::codecForName(prober.encoding())->toUnicode(data);
    }
    else {
        m_sourceContent = QString::fromUtf8(data);
    }


    emit sourceChanged();
    emit contentChanged();
    emit ready();
}

bool MyText::isPreview() const
{
    return m_isPreview;
}

void MyText::setPreview(bool preview)
{
    m_isPreview = preview;
    Qt::ScrollBarPolicy policy = m_isPreview ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAsNeeded;
    //m_viewer->setVerticalScrollBarPolicy(policy);
    //m_viewer->setHorizontalScrollBarPolicy(policy); TODO, FIXME: change those things in the delegate on the TextEdit
}
