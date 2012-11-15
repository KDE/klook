#ifndef LISTITEMCONTENT_H
#define LISTITEMCONTENT_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>

class File;
class KJob;

class ListItemContent : public QObject
{
    Q_OBJECT
public:
    explicit ListItemContent(File *file, QObject *parent = 0);
    virtual QVariant data(int role) const = 0;

    File *file() const;

signals:
    void dataChanged();

private:
    File *m_file;
};

class ListItemFallbackContent : public ListItemContent
{
    Q_OBJECT
public:
    explicit ListItemFallbackContent(File *file, QObject *parent = 0);

    virtual QVariant data(int role) const;

private slots:
    void handleStatJob(KJob *job);

private:
    QDateTime m_modificationTime;
    qulonglong m_size;
    bool m_statComplete;
};

class ListItemDirectoryContent : public ListItemFallbackContent
{
    Q_OBJECT
public:
    explicit ListItemDirectoryContent(File *file, QObject *parent = 0);

    virtual QVariant data(int role) const;

private slots:
    void handleDirSizeJob(KJob *job);

private:
    bool m_isScanned;
    qint64 m_totalFiles;
    qint64 m_totalSize;
    QDateTime m_modificationTime;
};

#endif // LISTITEMCONTENT_H
