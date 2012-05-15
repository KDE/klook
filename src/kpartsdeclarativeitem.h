#ifndef KPARTSDECLARATIVEITEM_H
#define KPARTSDECLARATIVEITEM_H

#include <QGraphicsProxyWidget>

#include "kpartswidget.h"

class KPartsDeclarativeItem : public QGraphicsProxyWidget
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit KPartsDeclarativeItem(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    ~KPartsDeclarativeItem();

signals:
    void urlChanged();

public slots:
    QString url() const;
    void setUrl(QString url);
    void setPartParent();

private:
    KPartsWidget* m_partWidget;
    QWidget *m_dummy;    
};

#endif // KPARTSDECLARATIVEITEM_H
