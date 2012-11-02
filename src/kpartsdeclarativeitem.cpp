#include "kpartsdeclarativeitem.h"

#include <QVBoxLayout>

KPartsDeclarativeItem::KPartsDeclarativeItem(QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    QGraphicsProxyWidget(parent, wFlags)
{    
    setWidget(NULL);

    m_dummy = new QWidget;
    m_partWidget = KPartsWidget::instance();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    m_dummy->setLayout(layout);

    setWidget(m_dummy);
}

KPartsDeclarativeItem::~KPartsDeclarativeItem()
{
    // for some reason when I try to delete KPartsWidget it just crashes
    // so delete only part and reuse widget object
    QObjectList list = m_dummy->children();
    if(list.size() > 1) {
        m_partWidget->setParent(0);
    }
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

void KPartsDeclarativeItem::setPartParent()
{
    m_dummy->layout()->addWidget(m_partWidget);
}
