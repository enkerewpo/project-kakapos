#-------------------------------------------------
#
# Project created by QtCreator 2017-08-07T12:32:09
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kakapos
TEMPLATE = app
SUBDIRS += Sources Headers
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cxx \
    src/AboutWindow.cxx \
    src/CodeEditor.cxx \
    src/HighlighterC.cxx \
    src/MainWindow.cxx


HEADERS += \
    include/headers.h \
    include/AboutWindow.h \
    include/CodeEditor.h \
    include/HighlighterC.h \
    include/MainWindow.h


FORMS += \
    ui/AboutWindow.ui \
    ui/MainWindow.ui

win32:{
    RC_ICONS = ui/kakapos.ico
    RC_FILE = ui/kakapos.rc
}

macx:{
    ICON = ui/Mac.icns
}

DISTFILES += \
    ui/kakapos.rc \
    config/kakapos_config.yml \
    plugin_framework/plugin_manager.py


#INCLUDEPATH += $$PWD/plugin_framework/include
#LIBS += -L$$PWD/plugin_framework/libs/ -lpython36
#DEPENDPATH += $$PWD/plugin_framework/libs
#PRE_TARGETDEPS += $$PWD/plugin_framework/libs/python36.lib

RESOURCES += \
    images.qrc
