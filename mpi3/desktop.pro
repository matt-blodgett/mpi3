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

HEADERS += desktop/ui/root.h
HEADERS += desktop/ui/plibrary.h
HEADERS += desktop/ui/pplayback.h

SOURCES += desktop/ui/root.cpp
SOURCES += desktop/ui/plibrary.cpp
SOURCES += desktop/ui/pplayback.cpp

HEADERS += desktop/ui/mvc/mlibitem.h
HEADERS += desktop/ui/mvc/mlibmodel.h
HEADERS += desktop/ui/mvc/mtreeview.h

SOURCES += desktop/ui/mvc/mlibitem.cpp
SOURCES += desktop/ui/mvc/mlibmodel.cpp
SOURCES += desktop/ui/mvc/mtreeview.cpp

HEADERS += util/uistyle.h
HEADERS += util/mpi3library.h
HEADERS += util/userprofile.h

SOURCES += util/uistyle.cpp
SOURCES += util/mpi3library.cpp
SOURCES += util/userprofile.cpp
