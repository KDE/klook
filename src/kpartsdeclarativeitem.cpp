#include "kpartsdeclarativeitem.h"

#include <QDebug>

KPartsDeclarativeItem::KPartsDeclarativeItem(QGraphicsItem * parent, Qt::WindowFlags wFlags   ) :
    QGraphicsProxyWidget(parent, wFlags)
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    m_partWidget = new KPartsWidget;
    m_partWidget->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );

    setWidget(m_partWidget);
}

KPartsDeclarativeItem::~KPartsDeclarativeItem()
{
    // set widget to null
    // for some reason when m_partWidgets is deleted segfault occurs
    // need to fix it later - it's memory leak

    setWidget(NULL);
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
