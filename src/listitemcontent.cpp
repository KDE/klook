#include "listitemcontent.h"
#include "listitem.h"
#include "file.h"

#include <kio/directorysizejob.h>
#include <KLocale>

ListItemContent::ListItemContent(File *file, QObject *parent)
    : QObject(parent)
    , m_file(file)
{
}

QVariant ListItemContent::data(int role) const
{
    return QVariant();
}

File *ListItemContent::file() const
{
    return m_file;
}

ListItemDirectoryContent::ListItemDirectoryContent(File *file, QObject *parent)
    : ListItemContent(file, parent)
    , m_isScanned(false)
    , m_totalFiles(0)
    , m_totalSize(0)
{
}

QVariant ListItemDirectoryContent::data(int role) const
{
    if(!m_isScanned) {
        KIO::DirectorySizeJob *job = KIO::directorySize(file()->url());
        connect(job, SIGNAL(result(KJob *)), SLOT(directorySizeResult(KJob *)));
        m_isScanned = true;
    }

    switch (role)
    {
    case ListItem::LastModifiedRole:
        //return KGlobal::locale()->formatDate( fi.lastModified().date() );
        return QVariant();
    case ListItem::ContentSizeRole:
        return KGlobal::locale()->formatByteSize(m_totalSize);

    case ListItem::CountRole:
        return QString::number(m_totalFiles);
    }

    return QVariant();
}

void ListItemDirectoryContent::directorySizeResult(KJob *job)
{
    KIO::DirectorySizeJob *dirSizeJob = dynamic_cast<KIO::DirectorySizeJob *>(job);
    if(dirSizeJob && !dirSizeJob->error()) {
        m_totalFiles = dirSizeJob->totalFiles();
        m_totalSize = dirSizeJob->totalSize();
        emit dataChanged();
    }
}

ListItemFallbackContent::ListItemFallbackContent(File *file, QObject *parent)
    : ListItemContent(file, parent)
{

}
#include <KIO/StatJob>
#include <QFileInfo>
#include <QDebug>
QVariant ListItemFallbackContent::data(int role) const
{
    qDebug() << file()->url();
    KIO::StatJob *job = KIO::stat(file()->url(), KIO::HideProgressInfo);
    connect(job, SIGNAL(result(KJob*)), SLOT(handleStatJob(KJob*)));

    if ( role == ListItem::LastModifiedRole )
    {
        QFileInfo fi( file()->url().toLocalFile() );
        return KGlobal::locale()->formatDate( fi.lastModified().date() );
    }
    else if ( role == ListItem::ContentSizeRole )
    {
        QFileInfo fi( file()->url().toLocalFile() );
        return KGlobal::locale()->formatByteSize( fi.size() );
    }

    return QVariant();
}

void ListItemFallbackContent::handleStatJob(KJob *job)
{
    const KIO::UDSEntry entry = static_cast<KIO::StatJob*>(job)->statResult();
    qDebug() << entry.stringValue(KIO::UDSEntry::UDS_MIME_TYPE);
    qDebug() << entry.stringValue(KIO::UDSEntry::UDS_MODIFICATION_TIME);
    const KUrl url = static_cast<KIO::StatJob*>(job)->url();
}
