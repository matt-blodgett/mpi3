DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=1
DEFINES += QT_DEBUG_PLUGINS=1
#DEFINES += QT_NO_WARNING_OUTPUT=1

QT += core
QT += xml
QT += gui
QT += widgets
QT += multimedia

TARGET = mpi3
TEMPLATE = app
RESOURCES += desktop.qrc

SOURCES += desktop/main.cpp \
    util/uistyle.cpp
SOURCES += desktop/rootwindow.cpp
HEADERS += desktop/rootwindow.h \
    util/uistyle.h

HEADERS +=
SOURCES +=

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

HEADERS +=
HEADERS += util/mpi3library.h
HEADERS += util/xmlsettings.h

SOURCES +=
SOURCES += util/mpi3library.cpp
SOURCES += util/xmlsettings.cpp
