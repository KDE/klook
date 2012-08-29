#include "progressdeclarativeitem.h"

#include <KIO/Job>
#include <kwidgetjobtracker.h>
#include <QtCore/QDebug>

KIO::Job *ProgressDeclarativeItem::m_job = 0;

ProgressDeclarativeItem::ProgressDeclarativeItem(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent, wFlags)
{
    if(!m_job)
        qDebug() << "Null job associated with declarative item";
    else
    {
        KWidgetJobTracker *jobTracker = new KWidgetJobTracker;
        jobTracker->registerJob(m_job);
        setWidget(jobTracker->widget(m_job));
    }
}

void ProgressDeclarativeItem::setJob(KIO::Job *job)
{
    m_job = job;
}
