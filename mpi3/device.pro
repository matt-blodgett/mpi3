DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT


CONFIG += c++17


QT += core
QT += gui
QT += widgets


TEMPLATE = app
VERSION = 1.0.0


TARGET = mpi3
QMAKE_TARGET_PRODUCT = "Mpi3Device"
QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"


target.path = /home/pi/Desktop/mpi3
INSTALLS += target


RESOURCES += assets/device.qrc


HEADERS += mglobal.h
SOURCES += mglobal.cpp

HEADERS += util/mstyle.h
SOURCES += util/mstyle.cpp
HEADERS += util/msettings.h
SOURCES += util/msettings.cpp

#HEADERS += core/maudioengine.h
#SOURCES += core/maudioengine.cpp
#HEADERS += core/mmedialibrary.h
#SOURCES += core/mmedialibrary.cpp

SOURCES += device/main.cpp
HEADERS += device/mroot.h
SOURCES += device/mroot.cpp

HEADERS += desktop/ui/mpanel.h
SOURCES += desktop/ui/mpanel.cpp
HEADERS += desktop/ui/mstatusbar.h
SOURCES += desktop/ui/mstatusbar.cpp
HEADERS += desktop/ui/mpanelhome.h
SOURCES += desktop/ui/mpanelhome.cpp
HEADERS += desktop/ui/mpanelmedia.h
SOURCES += desktop/ui/mpanelmedia.cpp
HEADERS += desktop/ui/mpanelplayback.h
SOURCES += desktop/ui/mpanelplayback.cpp

