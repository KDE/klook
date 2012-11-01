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
