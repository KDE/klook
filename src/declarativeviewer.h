/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef DECLARATIVEVIEWER_H
#define DECLARATIVEVIEWER_H

#include <QtDeclarative/QDeclarativeView>
#include <QVariant>
#include <QPair>

class QRect;
class PreviewGenerator;
class File;
class FileModel;
class QPoint;

enum WidgetRegion
{
    FRAME_REGION,
    HEADER_REGION,
    RIGHT_BORDER_REGION,
    LEFT_BORDER_REGION,
    TOP_BORDER_REGION,
    BOTTOM_BORDER_REGION,
    TOP_LEFT_CORNER_REGION,
    TOP_RIGHT_CORNER_REGION,
    BOTTOM_LEFT_CORNER_REGION,
    BOTTOM_RIGHT_CORNER_REGION,
    ARROW_NULL_REGION
};

typedef enum ArrowPosition
{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT

} ArrowPosition;

class DeclarativeViewer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit DeclarativeViewer(QWidget *parent = 0);
    virtual ~DeclarativeViewer();

    void init(QStringList urls, bool embedded = false, const QRect& rc = QRect(0, 0, 0, 0), int indexToShow = 0);
    Q_INVOKABLE void resizeToPreferredSize(int index);

    /**
     *
     * @brief Hack to pass video's size hint from QML to C++ side.
     *        To ensure that that size hint is used for right video
     *        index is passed along.
     * @param width
     * @param height
     * @param index
     */
    Q_INVOKABLE void setVideoSizeHint(int width, int height, int index);
    Q_INVOKABLE void setFullScreen();

    Q_INVOKABLE QString serviceForFile(int index) const;

    Q_INVOKABLE void updateCurrentFile(int index);

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void sizeChanged();

    /**
     * @brief Informs QML to set fullscreen state
     */
    void setFullScreenState();
    void setEmbeddedState();
    void setStartWindow();

public slots:
    void onSetGallery(bool);

private slots:
    void focusChanged(QWidget*, QWidget*);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    friend class KLookApp;
    enum ViewMode
    {
        Single,
        Multi
    };

    QSize getPreferredSize(const QString &path, int type) const;
    WidgetRegion calculateWindowRegion(const QPoint& mousePos);
    void setEmbedded(bool);

    void initModel(QStringList urls);
    void setViewMode(ViewMode mode);
    void registerTypes();
    QSize getTextWindowSize(QString url) const;
    void createVideoObject(QUrl url);

    void centerWidget(const QSize& sz);

    QPoint          m_lastMousePosition;
    bool            m_isSingleMode;
    bool            m_moving;
    bool            m_resize;
    bool            m_isEmbedded;
    bool            m_isGallery;
    QRect           m_rcIcon;
    QRect           m_rcArrow;

    File*     m_currentFile;

    WidgetRegion    m_region;

    FileModel*          m_fileModel;
    ArrowPosition       m_posArrow;

    QHash<int, QSize> m_videoSizeHints;
};

QSize calculateViewSize(const QSize& sz, const QRect &desktop);
QSize inscribedRectToRect(const QSize& sz1, const QSize& sz2);

#endif // DECLARATIVEVIEWER_H
