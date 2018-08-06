DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000


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
LIBS += $$PWD/lib/libav/avfilter.lib
LIBS += $$PWD/lib/libav/avutil.lib
INCLUDEPATH += $$PWD/lib/libav
DEPENDPATH += $$PWD/lib/libav


HEADERS += lib/libao/ao/ao.h

HEADERS += lib/libav/libavcodec/avcodec.h
HEADERS += lib/libav/libavformat/avformat.h
HEADERS += lib/libav/libavfilter/avfilter.h



RESOURCES += desktop.qrc
SOURCES += desktop/main.cpp

SOURCES += desktop/mroot.cpp
HEADERS += desktop/mroot.h

SOURCES += desktop/ui/maudiocontrol.cpp
SOURCES += desktop/ui/mlibrarydisplay.cpp

HEADERS += desktop/ui/maudiocontrol.h
HEADERS += desktop/ui/mlibrarydisplay.h

HEADERS += desktop/ui/mvc/mlibmodel.h
HEADERS += desktop/ui/mvc/mlibview.h
HEADERS += desktop/ui/mvc/mlibitem.h

SOURCES += desktop/ui/mvc/mlibmodel.cpp
SOURCES += desktop/ui/mvc/mlibview.cpp
SOURCES += desktop/ui/mvc/mlibitem.cpp

HEADERS += util/mstyle.h
HEADERS += util/mlibrary.h
HEADERS += util/msettings.h
HEADERS += util/maudio.h

SOURCES += util/mstyle.cpp
SOURCES += util/mlibrary.cpp
SOURCES += util/msettings.cpp
SOURCES += util/maudio.cpp
