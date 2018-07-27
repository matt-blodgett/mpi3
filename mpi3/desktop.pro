DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QT += core
QT += xml
QT += gui
QT += widgets
QT += multimedia

TARGET = Mpi3MediaPlayer
TEMPLATE = app

RESOURCES += desktop.qrc
RESOURCES += desktop/mpi3media

SOURCES += desktop/main.cpp

SOURCES += desktop/ui/root.cpp
SOURCES += desktop/ui/audiocontrol.cpp
SOURCES += desktop/ui/librarydisplay.cpp

HEADERS += desktop/ui/root.h
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
HEADERS += util/userprofile.h

SOURCES += util/uistyle.cpp
SOURCES += util/mpi3library.cpp
SOURCES += util/userprofile.cpp

HEADERS += util/xmlsettings.h
SOURCES += util/xmlsettings.cpp
