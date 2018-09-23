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


LIBS += $$PWD/lib/libao/libao.dll.a
INCLUDEPATH += $$PWD/lib/libao
DEPENDPATH += $$PWD/lib/libao


LIBS += $$PWD/lib/libav/avcodec.lib
LIBS += $$PWD/lib/libav/avformat.lib
LIBS += $$PWD/lib/libav/avresample.lib
LIBS += $$PWD/lib/libav/avutil.lib
INCLUDEPATH += $$PWD/lib/libav
DEPENDPATH += $$PWD/lib/libav


RESOURCES += fonts.qrc
RESOURCES += desktop.qrc
RC_FILE = desktop.rc


HEADERS += mglobal.h
SOURCES += mglobal.cpp

SOURCES += desktop/main.cpp
HEADERS += desktop/mroot.h
SOURCES += desktop/mroot.cpp

HEADERS += util/mstyle.h
SOURCES += util/mstyle.cpp
HEADERS += util/msettings.h
SOURCES += util/msettings.cpp

HEADERS += core/maudioengine.h
SOURCES += core/maudioengine.cpp
HEADERS += core/mmedialibrary.h
SOURCES += core/mmedialibrary.cpp

HEADERS += desktop/ui/maudiocontrol.h
SOURCES += desktop/ui/maudiocontrol.cpp
HEADERS += desktop/ui/mcontextbar.h
SOURCES += desktop/ui/mcontextbar.cpp
HEADERS += desktop/ui/mcontextpanel.h
SOURCES += desktop/ui/mcontextpanel.cpp
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
