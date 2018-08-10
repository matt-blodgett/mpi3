DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT


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
QMAKE_TARGET_PRODUCT = "Mpi3Desktop"
QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"


LIBS += $$PWD/lib/libao/libao.dll.a
INCLUDEPATH += $$PWD/lib/libao
DEPENDPATH += $$PWD/lib/libao


LIBS += $$PWD/lib/libav/avcodec.lib
LIBS += $$PWD/lib/libav/avformat.lib
LIBS += $$PWD/lib/libav/avresample.lib
LIBS += $$PWD/lib/libav/avutil.lib
INCLUDEPATH += $$PWD/lib/libav
DEPENDPATH += $$PWD/lib/libav


HEADERS += mglobal.h
SOURCES += mglobal.cpp

HEADERS += core/mlibrary.h
SOURCES += core/mlibrary.cpp

HEADERS += core/maudio.h
SOURCES += core/maudio.cpp

HEADERS += util/mstyle.h
SOURCES += util/mstyle.cpp

HEADERS += util/msettings.h
SOURCES += util/msettings.cpp

RESOURCES += desktop.qrc
SOURCES += desktop/main.cpp

HEADERS += desktop/mroot.h
SOURCES += desktop/mroot.cpp

HEADERS += desktop/ui/maudiocontrol.h
HEADERS += desktop/ui/mlibrarydisplay.h
SOURCES += desktop/ui/maudiocontrol.cpp
SOURCES += desktop/ui/mlibrarydisplay.cpp

HEADERS += desktop/ui/mvc/mlibitem.h
HEADERS += desktop/ui/mvc/mlibview.h
HEADERS += desktop/ui/mvc/mlibmodel.h
SOURCES += desktop/ui/mvc/mlibitem.cpp
SOURCES += desktop/ui/mvc/mlibview.cpp
SOURCES += desktop/ui/mvc/mlibmodel.cpp

