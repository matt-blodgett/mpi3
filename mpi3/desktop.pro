DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_WARNING_OUTPUT=0x010000
DEFINES += QT_FATAL_WARNINGS=0x000000
DEFINES += QT_DEBUG_PLUGINS=0x010000

CONFIG += c++1z

QT += core
QT += xml
QT += gui
QT += widgets
QT += multimedia

TEMPLATE = app
win32:VERSION = 1.0.0.1
else:VERSION = 1.0.0

TARGET = mpi3
QMAKE_TARGET_PRODUCT = "mpi3_desktop"
QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"

RESOURCES += desktop.qrc
SOURCES += desktop/main.cpp

SOURCES += desktop/rootwindow.cpp
HEADERS += desktop/rootwindow.h

SOURCES += desktop/panels/audiocontrol.cpp
SOURCES += desktop/panels/librarydisplay.cpp

HEADERS += desktop/panels/audiocontrol.h
HEADERS += desktop/panels/librarydisplay.h

HEADERS += desktop/mvc/libmodel.h
HEADERS += desktop/mvc/libview.h
HEADERS += desktop/mvc/libitem.h

SOURCES += desktop/mvc/libmodel.cpp
SOURCES += desktop/mvc/libview.cpp
SOURCES += desktop/mvc/libitem.cpp

HEADERS += util/uistyle.h
HEADERS += util/mpi3library.h
HEADERS += util/xmlsettings.h

SOURCES += util/uistyle.cpp
SOURCES += util/mpi3library.cpp
SOURCES += util/xmlsettings.cpp
