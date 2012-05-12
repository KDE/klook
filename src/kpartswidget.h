#ifndef KPARTSWIDGET_H
#define KPARTSWIDGET_H

#include <kparts/mainwindow.h>

class KPartsWidget : public KParts::MainWindow
{
    Q_OBJECT
public:
    KPartsWidget(QWidget *parent = 0);
    ~KPartsWidget();

    QString url() const { return m_part->url().url(); }
    void setUrl(const QString &url);

    QStringList supportedMimeTypes() const;

private:
    KParts::ReadOnlyPart *m_part;
};

#endif // KPARTSWIDGET_H
