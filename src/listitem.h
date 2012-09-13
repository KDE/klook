#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include "file.h"

class ListItemContent;

class ListItem : public QObject
{
    Q_OBJECT
public:
    enum
    {
        FilePathRole = Qt::UserRole + 1,
        FileNameRole,
        TypeRole,
        LastModifiedRole,
        ContentSizeRole,
        CountRole,       // object count for folders
        MimeRole,
        LocalFileRole, // is file local??
        DownloadInProgress
    };

    ListItem(File *file, QObject *parent);
    virtual ~ListItem() {}

    static QHash<int, QByteArray> roleNames();

    virtual QVariant data(int role) const;

    virtual QString path() const;
    void setPath(QString path) { m_file->setUrl(QUrl(path)); }

    virtual QString mime() const;

    virtual int type() const;
    void setType(int type) { m_file->setType(static_cast<File::FileType>(type)); }

    File *file() { return m_file; }

signals:
    void dataChanged();

private:
    File *m_file;
    mutable ListItemContent *m_content;
};

#endif // LISTITEM_H
