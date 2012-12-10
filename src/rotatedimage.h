#ifndef ROTATEDIMAGE_H
#define ROTATEDIMAGE_H

/* Explanation extracted from http://sylvana.net/jpegcrop/exif_orientation.html

   For convenience, here is what the letter F would look like if it were tagged
correctly and displayed by a program that ignores the orientation tag (thus
showing the stored image):

  1        2       3      4         5            6           7          8

888888  888888      88  88      8888888888  88                  88  8888888888
88          88      88  88      88  88      88  88          88  88      88  88
8888      8888    8888  8888    88          8888888888  8888888888          88
88          88      88  88
88          88  888888  888888

*/

#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QMatrix>

enum Orientation {
    NOT_AVAILABLE = 0,
    NORMAL = 1,
    HFLIP = 2,
    ROT_180 = 3,
    VFLIP = 4,
    TRANSPOSE = 5,
    ROT_90 = 6,
    TRANSVERSE = 7,
    ROT_270 = 8
};

class RotatedImage
{
public:
    RotatedImage(const QString& path);

    QMatrix rotationMatrix() const;

private:
    void rotateImage();
    QMatrix createRotMatrix(int angle);
    QMatrix createScaleMatrix(int dx, int dy);

    QMatrix m_matrix;
    QSize m_size;
    QString m_path;
};

#endif // ROTATEDIMAGE_H
