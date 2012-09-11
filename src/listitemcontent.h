#ifndef LISTITEMCONTENT_H
#define LISTITEMCONTENT_H

#include <QObject>

class File;
class KJob;

class ListItemContent : public QObject
{
    Q_OBJECT
public:
    explicit ListItemContent(File *file, QObject *parent = 0);

    virtual QVariant data(int role) const;

    File *file() const;

signals:
    void dataChanged();

private:
    File *m_file;
};

class ListItemDirectoryContent : public ListItemContent
{
    Q_OBJECT
public:
    explicit ListItemDirectoryContent(File *file, QObject *parent = 0);

    virtual QVariant data(int role) const;

private slots:
    void directorySizeResult(KJob *job);
private:
    mutable bool m_isScanned;
    qint64 m_totalFiles;
    qint64 m_totalSize;
};

class ListItemFallbackContent : public ListItemContent
{
    Q_OBJECT
public:
    explicit ListItemFallbackContent(File *file, QObject *parent = 0);

    virtual QVariant data(int role) const;

private slots:
    void handleStatJob(KJob *job);
};


#endif // LISTITEMCONTENT_H
