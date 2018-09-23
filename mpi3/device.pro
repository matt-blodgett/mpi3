DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT


CONFIG += c++17


QT += core
QT += gui
QT += xml
QT += widgets


TEMPLATE = app
VERSION = 1.0.0


TARGET = mpi3
QMAKE_TARGET_PRODUCT = "Mpi3Device"
QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"


target.path = /home/pi/device/mpi3
INSTALLS += target


RESOURCES += assets/device.qrc


#LIBS += $$PWD/lib/libao/libao.dll.a
INCLUDEPATH += $$PWD/lib/libao
DEPENDPATH += $$PWD/lib/libao


#LIBS += $$PWD/lib/libav/avcodec.lib
#LIBS += $$PWD/lib/libav/avformat.lib
#LIBS += $$PWD/lib/libav/avresample.lib
#LIBS += $$PWD/lib/libav/avutil.lib
INCLUDEPATH += $$PWD/lib/libav
DEPENDPATH += $$PWD/lib/libav


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

SOURCES += device/main.cpp
HEADERS += device/mroot.h
SOURCES += device/mroot.cpp

HEADERS += device/ui/mpanel.h
SOURCES += device/ui/mpanel.cpp
HEADERS += device/ui/mstatusbar.h
SOURCES += device/ui/mstatusbar.cpp
HEADERS += device/ui/mpanelhome.h
SOURCES += device/ui/mpanelhome.cpp
HEADERS += device/ui/mpanelmedia.h
SOURCES += device/ui/mpanelmedia.cpp
HEADERS += device/ui/mpanelplayback.h
SOURCES += device/ui/mpanelplayback.cpp

