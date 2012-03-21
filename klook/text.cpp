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

#include <QPlainTextEdit>
#include <QTextStream>
#include <QFile>
#include <QDebug>

MyText::MyText( QGraphicsItem* parent )
    : QGraphicsProxyWidget( parent )
    , m_source( "" )
    , m_isPreview( false )
{
    m_viewer = new QPlainTextEdit();
    m_viewer->setReadOnly( true );
    m_viewer->viewport()->setCursor( Qt::ArrowCursor );

    m_viewer->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );

    setWidget( m_viewer );
}

MyText::~MyText()
{
//    if ( m_viewer ) delete m_viewer;
}

QString MyText::source() const
{
    return m_source;
}

void MyText::setSource( const QString& source )
{
    if ( m_source == source )
        return;
    m_source = source;
    QFile f( m_source );
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) )
        return;
    QString str = f.readAll();
    m_viewer->setPlainText( str );
    emit sourceChanged();
    emit ready();
}

bool MyText::isPreview() const
{
    return m_isPreview;
}

void MyText::setPreview( bool preview )
{
    m_isPreview = preview;

    if ( m_isPreview )
    {
        m_viewer->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        m_viewer->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    }
    else
    {
        m_viewer->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
        m_viewer->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    }
}

void MyText::slot1()
{
    qDebug() << "ideal 2" << m_viewer->sizeHint();
    qDebug() << "ideal 3" << m_viewer->document()->documentLayout()->documentSize().toSize() << m_viewer->sizePolicy();
    m_viewer->resize(m_viewer->document()->documentLayout()->documentSize().toSize());
}
