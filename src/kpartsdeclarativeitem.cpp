#include "kpartsdeclarativeitem.h"
#include "kpartswidget.h"

#include <QVBoxLayout>

KPartsDeclarativeItem::KPartsDeclarativeItem(QGraphicsItem * parent, Qt::WindowFlags wFlags   ) :
    QGraphicsProxyWidget(parent, wFlags)
{
    m_partWidget = new KPartsWidget;
    m_partWidget->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );

    setWidget(m_partWidget);
}

KPartsDeclarativeItem::~KPartsDeclarativeItem()
{
    delete m_partWidget;
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
