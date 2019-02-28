QT += core
QT += gui
QT += widgets

INCLUDEPATH += ../core
INCLUDEPATH += ../util

include(ui/ui.pri)

INCLUDEPATH += $$PWD
HEADERS += $$PWD/mdevice.h

HEADERS += $$PWD/mroot.h
SOURCES += $$PWD/mroot.cpp
