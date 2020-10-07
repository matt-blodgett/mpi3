QT += core
QT += gui
QT += widgets

INCLUDEPATH += ../../

include(ui/ui.pri)

INCLUDEPATH += $$PWD

HEADERS += $$PWD/mdevice.h

HEADERS += $$PWD/mroot.h
SOURCES += $$PWD/mroot.cpp
