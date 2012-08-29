#ifndef PROGRESSDECLARATIVEITEM_H
#define PROGRESSDECLARATIVEITEM_H

#include <QtGui/QGraphicsProxyWidget>

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

private:
    static KIO::Job *m_job;
};

#endif // PROGRESSDECLARATIVEITEM_H
