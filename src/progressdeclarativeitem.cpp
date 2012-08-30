#include "progressdeclarativeitem.h"

#include <KIO/Job>
#include <kwidgetjobtracker.h>
#include <QWidget>

QPointer<KIO::Job> ProgressDeclarativeItem::m_job = 0;

ProgressDeclarativeItem::ProgressDeclarativeItem(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent, wFlags)
{
    layout = new QVBoxLayout;
    layout->setMargin( 0 );
    QWidget *m_dummy = new QWidget;
    m_dummy->setLayout(layout);

    setWidget(m_dummy);

}

void ProgressDeclarativeItem::setJob(KIO::Job *job)
{
    m_job = job;
}

void ProgressDeclarativeItem::setJobWidget()
{
    if(m_job) {
        KWidgetJobTracker *jobTracker = new KWidgetJobTracker;
        jobTracker->registerJob(m_job);
        layout->addWidget(jobTracker->widget(m_job));
    }
}
