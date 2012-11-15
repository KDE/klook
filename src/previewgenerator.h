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

#ifndef PREVIEWGENERATOR_H
#define PREVIEWGENERATOR_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtGui/QPixmap>

class FileModel;
class KFileItem;

class PreviewGenerator : public QObject
{
    Q_OBJECT
public:
    void setModel(FileModel *model);

    QPixmap takePreviewPixmap(QString filePath);

    /**
     * @brief modelShown is called in QML after "cloud" is loaded. It generates
     *        previews and notifies model when previews are generated
     * @param dayModel
     */
    Q_INVOKABLE void request(const QString &path, const QSize &size);

    /**
     * @brief modelHidden is in QML when cloud component gets destroyed. It removes
     *        unused  images generated for model
     * @param dayModel
     */
    Q_INVOKABLE void cancel(const QString &path);

    static PreviewGenerator *instance();

private slots:
    void previewJobResult(const KFileItem&, const QPixmap&);
    void previewJobFailed(const KFileItem &item);

private:
    friend PreviewGenerator *previewGenerator(const QString &type);

    explicit PreviewGenerator();
    void notifyModelAboutPreview(const QString &url);

    QHash<QString, QPixmap> m_previews;

    QPixmap videoPixmap;
    QStringList m_plugins;
    FileModel *m_model;
};
#endif // PREVIEWGENERATOR_H
