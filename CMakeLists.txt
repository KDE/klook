project( klook )

find_package( KDE4 REQUIRED )
include( KDE4Defaults )

find_package( Qt4 REQUIRED QtCore QtGui QtDeclarative )
include( ${QT_USE_FILE} )
qt4_automoc( ${qtproject_SRCS} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

include_directories( ${KDE4_INCLUDES} ${QT_QDECLARATIVE_INCLUDE_DIR} )

set( klook_SRCS src/main.cpp
				src/declarativeviewer.cpp
				src/video.cpp
				src/text.cpp
				src/file.cpp
				src/workerthread.cpp
				src/previewgenerator.cpp
				src/previewprovider.cpp
				src/filemodel.cpp
				src/klookapp.cpp
				src/audio.cpp
				src/mimeprovider.cpp )

kde4_add_executable( klook ${klook_SRCS} )

target_link_libraries( klook ${KDE4_KIO_LIBS}
							${KDE4_PHONON_LIBS}
 							${KDE4_PLASMA_LIBS}
							${KDE4_KFILE_LIBS}
							${QT_LIBRARIES}
							${QT_QDECLARATIVE_LIBRARIES} )

install( TARGETS klook ${INSTALL_TARGETS_DEFAULT_ARGS} )

file( GLOB qml_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml" )
install( FILES ${qml_files} DESTINATION share/${PROJECT_NAME}/ )
 
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/qml/images/
		 DESTINATION share/${PROJECT_NAME}/images )

#Translations
find_package(Msgfmt REQUIRED)
find_package(Gettext REQUIRED)
add_subdirectory(locale)