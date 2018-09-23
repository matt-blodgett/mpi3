DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT


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


#HEADERS += mglobal.h
#SOURCES += mglobal.cpp

SOURCES += device/main.cpp
HEADERS += device/mroot.h
SOURCES += device/mroot.cpp
