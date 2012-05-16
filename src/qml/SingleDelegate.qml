
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

import QtQuick 1.1
import Widgets 1.0

Item {
    id: listItem
    width: photosListView.width; height: photosListView.height

    function getHeight( parentHeight, iconHeight)
    {
        var h = ( parentHeight - panel.height ) * 4 / 5
        if ( h > iconHeight )
            h = iconHeight
        return h
    }

    function getName( path )
    {
        if ( path.substr( -1 ) === '/' && path.length !== 1 ) { // remove trailing slash if it's not root
            path = path.substr( 0, path.length - 1 );
        }
        return path.substring( path.lastIndexOf( '/' ) + 1 );
    }

    ImageDelegate {
        id: imgDelegate
    }

    VideoDelegate {
        id: videoDelegate
    }

    AudioDelegate {
        id: audioDelegate
    }

    TextDelegate {
        id: txtDelegate
    }

    FolderDelegate {
        id: folderDelegate
    }

    MimeDelegate {
        id: mimeDelegate
    }

    Item {
        OkularDelegate {
            id: okularDelegate
        }
    }

    // function for getting delegate of loader element
    function bestDelegate( t ) {
        if ( t == 1 )
            return imgDelegate;
        else if ( t == 2 )
            return videoDelegate;
        else if ( t == 3 )
            return txtDelegate;
        else if ( t == 4 ) {
            fileModel.scanDirectory( index )
            return folderDelegate;
        }
        else if ( t == 5 )
            return audioDelegate;
        else if ( t == 6) {
            return okularDelegate;
        }
        else if(t == 0)
            return mimeDelegate;
    }

    Loader
    {
        id: componentLoader
        anchors.fill: parent;
        sourceComponent: bestDelegate( type )
    }
}

