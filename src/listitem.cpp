#include "listitem.h"
#include "listitemcontent.h"
#include "filemodel.h"

ListItem::ListItem(File *file, QObject *parent)
    : QObject(parent), m_file(file), m_content(0)
{
    if(!m_file->url().isLocalFile())
        connect(file, SIGNAL(loaded()), SIGNAL(dataChanged()));
    connect(file, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
}

ListItem *ListItem::newItem(File *file, QObject *parent)
{
    if(file->type() == File::MimetypeFallback)
        return new UnsupportedItem(file, parent);
    return new ListItem(file, parent);
}

QString ListItem::path() const
{
    QString result;
    // if file is remote and if it was copied on hard disk
    // return path to temporary file
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
    // instantiate content only when roles really require it
    if(!m_content && role != FilePathRole
            && role != LocalFileRole
            && role != FileNameRole) {
        const int type = m_file->type();
        /* ugly. need to fix later */
        QObject *parent = const_cast<QObject *>(qobject_cast<const QObject *>(this));
        if(type == File::Directory)
            m_content = new ListItemDirectoryContent(m_file, parent);
        else if(type == File::MimetypeFallback)
            m_content = new ListItemFallbackContent(m_file, parent);
        else
            m_content = new ListItemContent(m_file, parent);
        connect(m_content, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
    }

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

    default:
        return m_content->data(role);
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> ListItem::roleNames()
{
    QHash<int, QByteArray> names;
    names[FilePathRole] = "filePath";
    names[FileNameRole] = "fileName";
    names[TypeRole] = "type";
    names[LastModifiedRole] = "lastModified";
    names[ContentSizeRole] = "contentSize";
    names[CountRole] = "countElements";
    names[MimeRole] = "mime";
    names[LocalFileRole] = "isLocal";

    return names;
}

