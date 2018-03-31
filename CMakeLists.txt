project( klook )
cmake_minimum_required( VERSION 3.5.1 FATAL_ERROR )
set (QT_MIN_VERSION "5.9.0")
set (KF5_MIN_VERSION "5.35.0") # TODO: find an appropriate version


find_package(ECM ${KF5_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations") # Yeah, everything is deprecated, we know

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

kde_enable_exceptions() # We actually use/handle exceptions

find_package( Qt5 ${QT_MIN_VERSION}  NO_MODULE REQUIRED Core Gui Quick Qml)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    KDELibs4Support
    KIO
    Parts
    Declarative
    Plasma
)

find_package(Phonon4Qt5 REQUIRED)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory( icons )

set(EXIV2_MIN_VERSION "0.19")
find_package(Exiv2)

set( QRC_RESOURCES src/resources.qrc )

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
                src/exifimageprovider.cpp
                src/rotatedimage.cpp
                ${qml_files} )

#set_source_files_properties(src/rotatedimage.cpp PROPERTIES COMPILE_FLAGS ${KDE4_ENABLE_EXCEPTIONS})

file(GLOB ICON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/hi*-apps-klook.png" )
ecm_add_app_icon(klook_SRCS ICONS ${ICON_SRCS})

add_executable( klook ${klook_SRCS} ${QRC_SOURCES} )

target_include_directories(klook SYSTEM PUBLIC ${PHONON_INCLUDE_DIR})

target_link_libraries(klook KF5::KIOCore KF5::KIOFileWidgets KF5::KIOWidgets KF5::KIONTLM
                            ${EXIV2_LIBRARIES}
                            KF5::Plasma
                            ${PHONON_LIBRARY}
                            KF5::Parts
                            KF5::Declarative
                            KF5::KDELibs4Support
                            Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick
                     )


install( TARGETS klook ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${qml_files} DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/ )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/qml/images/
                 DESTINATION ${DATA_INSTALL_DIR}/${PROJECT_NAME}/images )

#FIX AT SOME OTHER POINT IN TIME
#Translations
#if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/locale)
  #find_package( Msgfmt REQUIRED )
  #find_package( Gettext REQUIRED )
  #add_subdirectory( locale )
#endif()

