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

SOURCES += desktop/main.cpp
SOURCES += desktop/rootwindow.cpp
HEADERS += desktop/rootwindow.h

SOURCES += desktop/ui/uistyle.cpp
SOURCES += desktop/ui/audiocontrol.cpp
SOURCES += desktop/ui/librarydisplay.cpp

HEADERS += desktop/ui/uistyle.h
HEADERS += desktop/ui/audiocontrol.h
HEADERS += desktop/ui/librarydisplay.h

HEADERS += desktop/mvc/libmodel.h
HEADERS += desktop/mvc/libview.h
HEADERS += desktop/mvc/libitem.h

SOURCES += desktop/mvc/libmodel.cpp
SOURCES += desktop/mvc/libview.cpp
SOURCES += desktop/mvc/libitem.cpp

HEADERS += util/uistylesheet.h
HEADERS += util/mpi3library.h
HEADERS += util/xmlsettings.h

SOURCES += util/uistylesheet.cpp
SOURCES += util/mpi3library.cpp
SOURCES += util/xmlsettings.cpp
