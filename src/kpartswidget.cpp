#include "kpartswidget.h"

#include <KToolBar>
#include <QtGui/QDesktopWidget>

KPartsWidget *KPartsWidget::m_instance = 0;

KPartsWidget::KPartsWidget(QWidget *parent)
    : KParts::MainWindow(parent, KDE_DEFAULT_WINDOWFLAGS)
{
    m_service = KService::serviceByDesktopPath("okular_part.desktop");

    if( m_service )
    {
        m_part = m_service->createInstance<KParts::ReadOnlyPart>(0, QVariantList() << "Print/Preview");
        if(m_part)
        {
            setCentralWidget(m_part->widget());
            // we don't need toolabrs being shown
            foreach(KToolBar *toolbar, toolBars())
                toolbar->hide();
        }
    }
}

KPartsWidget *KPartsWidget::instance()
{
    if(!m_instance)
        m_instance = new KPartsWidget;

    return m_instance;
}

void KPartsWidget::setUrl(const QString &url)
{
    if(m_part)
        m_part->openUrl(url);
}

QStringList KPartsWidget::supportedMimeTypes() const
{
    return QStringList();
}

QSize getDocumentWindowSize()
{
    QDesktopWidget dw;
    QSize size;

    if (dw.height() < dw.width()) {
        size.setHeight(dw.height() * 0.8);
        size.setWidth(size.height () / 1.4142 + 40  * 1.4142);
    }
    else {
        size.setWidth(dw.width() * 0.8);
        size.setHeight(size.width() * 1.4142);
        size.setWidth(size.width() + 40); // caption
    }
    return size;
}
