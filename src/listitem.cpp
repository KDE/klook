#include "listitem.h"
#include "listitemcontent.h"
#include "filemodel.h"

ListItem::ListItem(File *file, QObject *parent)
    : QObject(parent), m_file(file), m_content(0)
{
    connect(file, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
}

QString ListItem::path() const
{
    QString result;
    if(!m_file->url().isLocalFile() && m_file->isLoaded())
        result = m_file->tempFilePath();
    else
        result = m_file->url().pathOrUrl();

    return result;
}

int ListItem::type() const
{
    return m_file->type();
}

QString ListItem::mime() const
{
    return m_file->mime();
}

QVariant ListItem::data( int role ) const
{
    switch ( role )
    {
    case FilePathRole:
        return path();

    case TypeRole:
        return type();

    case MimeRole:
        return mime();

    case LocalFileRole:
        return m_file->url().isLocalFile();

    case FileNameRole:
        return m_file->url().fileName();

    case DownloadInProgress:
        return m_file->downloadInProgress();

    case UrlRole:
        return m_file->url().url();

    case LoadedRole:
        return m_file->isLoaded();

    default:
        break;
    }

    // instantiate content only when roles really require it
    if(!m_content) {
        const int type = m_file->type();
        QObject *parent = const_cast<QObject *>(qobject_cast<const QObject *>(this));
        if(type == File::Directory)
            m_content = new ListItemDirectoryContent(m_file, parent);
        else
            m_content = new ListItemFallbackContent(m_file, parent);
        connect(m_content, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
    }

    return m_content->data(role);
}

QHash<int, QByteArray> ListItem::roleNames()
{
    QHash<int, QByteArray> names;
    names[FilePathRole] = "filePath";
    names[FileNameRole] = "fileName";
    names[UrlRole] = "fileUrl";
    names[TypeRole] = "type";
    names[LastModifiedRole] = "lastModified";
    names[ContentSizeRole] = "contentSize";
    names[CountRole] = "countElements";
    names[MimeRole] = "mime";
    names[LoadedRole] = "isLoaded";
    names[LocalFileRole] = "isLocal";
    names[DownloadInProgress] = "downloadInProgress";

    return names;
}
