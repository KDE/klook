#ifndef KPARTSWIDGET_H
#define KPARTSWIDGET_H

#include <kparts/mainwindow.h>

#include <QPointer>
#include <kservice.h>

class KPartsWidget : public KParts::MainWindow
{
    Q_OBJECT
public:
    KPartsWidget(QWidget *parent = 0);

    QString url() const { return m_part->url().url(); }
    void setUrl(const QString &url);

    QStringList supportedMimeTypes() const;

private:
    QPointer<KParts::ReadOnlyPart> m_part;
    KService::Ptr m_service;
};

QSize getDocumentWindowSize();

#endif // KPARTSWIDGET_H
