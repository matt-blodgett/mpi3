DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT


CONFIG += c++1z


QT += core
QT += gui
QT += xml
QT += widgets
QT += multimedia


TEMPLATE = app
win32:VERSION = 1.0.0.1
else:VERSION = 1.0.0


TARGET = mpi3
QMAKE_TARGET_PRODUCT = "Mpi3Desktop"
QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"


RESOURCES += assets/fonts.qrc
RESOURCES += assets/desktop.qrc
RC_FILE = assets/desktop.rc

#http://doc.qt.io/archives/qt-4.8/qmake-advanced-usage.html

win32 {
    LIBS += $$PWD/lib/libao/libao.dll.a
    LIBS += $$PWD/lib/ffmpeg/avcodec.lib
    LIBS += $$PWD/lib/ffmpeg/avformat.lib
    LIBS += $$PWD/lib/ffmpeg/avutil.lib

    LIBS += $$PWD/lib/libao/libao.dll.a
    INCLUDEPATH += $$PWD/lib/libao
    DEPENDPATH += $$PWD/lib/libao


    LIBS += $$PWD/lib/ffmpeg/avcodec.lib
    LIBS += $$PWD/lib/ffmpeg/avformat.lib
    LIBS += $$PWD/lib/ffmpeg/avutil.lib
    INCLUDEPATH += $$PWD/lib/ffmpeg
    DEPENDPATH += $$PWD/lib/ffmpeg

}


unix {
    LIBS += -lao
    LIBS += -lswresample
    LIBS += -lswscale
    LIBS += -lpostproc
    LIBS += -lavutil
    LIBS += -lavcodec
    LIBS += -lavformat
}


HEADERS += mglobal.h
SOURCES += mglobal.cpp

HEADERS += util/mstyle.h
SOURCES += util/mstyle.cpp
HEADERS += util/msettings.h
SOURCES += util/msettings.cpp

HEADERS += core/maudioengine.h
SOURCES += core/maudioengine.cpp
HEADERS += core/mmedialibrary.h
SOURCES += core/mmedialibrary.cpp

SOURCES += desktop/main.cpp
HEADERS += desktop/mroot.h
SOURCES += desktop/mroot.cpp

HEADERS += desktop/ui/mpanel.h
SOURCES += desktop/ui/mpanel.cpp
HEADERS += desktop/ui/mcontextbar.h
SOURCES += desktop/ui/mcontextbar.cpp
HEADERS += desktop/ui/mpanelplayback.h
SOURCES += desktop/ui/mpanelplayback.cpp
HEADERS += desktop/ui/mpanellibrary.h
SOURCES += desktop/ui/mpanellibrary.cpp
HEADERS += desktop/ui/mpaneldevice.h
SOURCES += desktop/ui/mpaneldevice.cpp
HEADERS += desktop/ui/mpanelmedia.h
SOURCES += desktop/ui/mpanelmedia.cpp

HEADERS += desktop/ui/mvc/mlibitem.h
SOURCES += desktop/ui/mvc/mlibitem.cpp
HEADERS += desktop/ui/mvc/mlibview.h
SOURCES += desktop/ui/mvc/mlibview.cpp
HEADERS += desktop/ui/mvc/mlibmodel.h
SOURCES += desktop/ui/mvc/mlibmodel.cpp
HEADERS += desktop/ui/mvc/mdrivemodel.h
SOURCES += desktop/ui/mvc/mdrivemodel.cpp
