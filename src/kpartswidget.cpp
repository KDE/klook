#include "kpartswidget.h"

#include <kservice.h>
#include <ktoolbar.h>

KPartsWidget::KPartsWidget(QWidget *parent)
    : KParts::MainWindow(parent, KDE_DEFAULT_WINDOWFLAGS)
{
    KService::Ptr service = KService::serviceByDesktopPath("okular_part.desktop");

    if( service )
    {
        m_part = service->createInstance<KParts::ReadOnlyPart>(0, QVariantList() << "Print/Preview");

        if(m_part)
        {
            setCentralWidget(m_part->widget());

            // we don't need toolabrs being shown
            foreach(KToolBar *toolbar, toolBars())
                toolbar->hide();
        }
    }
}

KPartsWidget::~KPartsWidget()
{

}

void KPartsWidget::setUrl(const QString &url)
{
    m_part->openUrl(url);
    show();
}

QStringList KPartsWidget::supportedMimeTypes() const
{
    return QStringList();
}
