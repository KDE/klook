#include "mimetypeimageprovider.h"

#include <kicon.h>
#include <kmimetype.h>

MimeTypeImageProvider::MimeTypeImageProvider() :
    QDeclarativeImageProvider(Pixmap)
{
}

QPixmap MimeTypeImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    KIcon icon(KMimeType::iconNameForUrl(id));
    QPixmap pixmap;
    pixmap = icon.isNull() ? QPixmap( ":images/pla-empty-box.png" ) : icon.pixmap(1000);
    if ( requestedSize.isValid() )
        pixmap = pixmap.scaled( requestedSize, Qt::KeepAspectRatio,Qt::SmoothTransformation );
    *size = pixmap.size();

    return pixmap;
}
