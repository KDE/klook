#ifndef MIMETYPEIMAGEPROVIDER_H
#define MIMETYPEIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

class MimeTypeImageProvider : public QDeclarativeImageProvider
{
public:
    MimeTypeImageProvider();
    QPixmap requestPixmap( const QString& id, QSize* size, const QSize& requestedSize );
};

#endif // MIMETYPEIMAGEPROVIDER_H
