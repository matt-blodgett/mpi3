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

SOURCES += desktop/ui/rootwindow.cpp
SOURCES += desktop/ui/audiocontrol.cpp
SOURCES += desktop/ui/librarydisplay.cpp

HEADERS += desktop/ui/rootwindow.h
HEADERS += desktop/ui/audiocontrol.h
HEADERS += desktop/ui/librarydisplay.h

HEADERS += desktop/ui/mvc/libmodel.h
HEADERS += desktop/ui/mvc/libview.h
HEADERS += desktop/ui/mvc/libitem.h

SOURCES += desktop/ui/mvc/libmodel.cpp
SOURCES += desktop/ui/mvc/libview.cpp
SOURCES += desktop/ui/mvc/libitem.cpp

HEADERS += util/uistyle.h
HEADERS += util/mpi3library.h
HEADERS += util/xmlsettings.h

SOURCES += util/uistyle.cpp
SOURCES += util/mpi3library.cpp
SOURCES += util/xmlsettings.cpp
