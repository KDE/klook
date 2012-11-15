#ifndef KPARTSDECLARATIVEITEM_H
#define KPARTSDECLARATIVEITEM_H

#include <QtGui/QGraphicsProxyWidget>

class KPartsWidget;

class KPartsDeclarativeItem : public QGraphicsProxyWidget
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
public:
    explicit KPartsDeclarativeItem(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);

    Q_INVOKABLE QString url() const;
    Q_INVOKABLE void setUrl(QString url);

signals:
    void urlChanged();

private:
    KPartsWidget *m_partWidget;
};

#endif // KPARTSDECLARATIVEITEM_H
