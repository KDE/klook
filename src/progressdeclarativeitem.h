#ifndef PROGRESSDECLARATIVEITEM_H
#define PROGRESSDECLARATIVEITEM_H

#include <QtCore/QPointer>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QVBoxLayout>

namespace KIO
{
    class Job;
}

class ProgressDeclarativeItem : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    explicit ProgressDeclarativeItem(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);

    static void setJob(KIO::Job *job);

public slots:
    void setJobWidget();

private:
    static QPointer<KIO::Job> m_job;
    QVBoxLayout *layout;
};

#endif // PROGRESSDECLARATIVEITEM_H
