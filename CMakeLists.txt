project( klook )
cmake_minimum_required( VERSION 2.6 )
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0017 NEW)
endif(COMMAND cmake_policy)

find_package( KDE4 REQUIRED )
include( KDE4Defaults )

find_package( Qt4 REQUIRED QtCore QtGui QtDeclarative )
include( ${QT_USE_FILE} )
qt4_automoc( ${qtproject_SRCS} )
add_definitions( ${QT_DEFINITIONS} )

include_directories( ${KDE4_INCLUDES} ${QT_QDECLARATIVE_INCLUDE_DIR} )
add_subdirectory( icons )

set( RESOURCES src/resources.qrc )
QT4_ADD_RESOURCES( QRC_SOURCES ${RESOURCES} )

file( GLOB qml_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml" )

set( klook_SRCS src/main.cpp
                src/declarativeviewer.cpp
                src/video.cpp
                src/text.cpp
                src/file.cpp
                src/listitem.cpp
                src/previewgenerator.cpp
                src/previewprovider.cpp
                src/filemodel.cpp
                src/klookapp.cpp
                src/audio.cpp
                src/mimeprovider.cpp
                src/kpartsdeclarativeitem.cpp
                src/kpartswidget.cpp
                src/listitemcontent.cpp
                ${qml_files} )

#set( KDE4_ICON_DIR  ${CMAKE_INSTALL_PREFIX}/share/icons )

kde4_add_app_icon( klook_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hi*-apps-klook.png" )
kde4_add_executable( klook ${klook_SRCS} ${QRC_SOURCES} )

target_link_libraries( klook ${KDE4_KIO_LIBS}
                           ${KDE4_PHONON_LIBS}
                           ${KDE4_PLASMA_LIBS}
                           ${KDE4_KFILE_LIBS}
                           kparts
                           ${QT_LIBRARIES}
                           ${QT_QDECLARATIVE_LIBRARIES}
                      )


install( TARGETS klook ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${qml_files} DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/ )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/qml/images/
                 DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/images )

#Translations
find_package( Msgfmt REQUIRED )
find_package( Gettext REQUIRED )
add_subdirectory( locale )

