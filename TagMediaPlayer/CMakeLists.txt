cmake_minimum_required(VERSION 3.5)

project(TagMediaPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)


include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../EZFrameless/src
    ${CMAKE_SOURCE_DIR}/../EZFrameless/qutils/src
)


set(PROJECT_SOURCES
        main.cpp
        TagMediaPlayer.cpp
        TagMediaPlayer.h
        TagMediaPlayer.ui
        PlayerWidget.h
        PlayerWidget.cpp
        PlayerWidget.ui
        SignalMacros.h
        CustomVideoWidget.h
        CustomVideoWidget.cpp
        TitleBar.h
        TitleBar.cpp
        TitleBar.ui
        CusVideoWidget.h
        CusVideoWidget.cpp
)

set(OTHER_SOURCES
    ${CMAKE_SOURCE_DIR}/../EZFrameless/src/EZFrameless.h
    ${CMAKE_SOURCE_DIR}/../EZFrameless/src/EZFrameless.cpp
    ${CMAKE_SOURCE_DIR}/../EZFrameless/src/EZTitleBarBase.h
    ${CMAKE_SOURCE_DIR}/../EZFrameless/qutils/src/FontManager.h
    ${CMAKE_SOURCE_DIR}/../EZFrameless/qutils/src/FontManager.cpp
    ${CMAKE_SOURCE_DIR}/../EZFrameless/qutils/src/QEventForwarder.h
    ${CMAKE_SOURCE_DIR}/../EZFrameless/qutils/src/QEventForwarder.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TagMediaPlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${OTHER_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TagMediaPlayer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TagMediaPlayer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TagMediaPlayer
            ${PROJECT_SOURCES}
            ${OTHER_SOURCES}
        )
    endif()
endif()

target_link_libraries(TagMediaPlayer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::MultimediaWidgets dwmapi user32)

set_target_properties(TagMediaPlayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS TagMediaPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TagMediaPlayer)
endif()
