QT += core
QT += xml
QT += gui
QT += widgets
QT += multimedia

TARGET = Mpi3MediaPlayer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

HEADERS += desktop/ui/mroot.h
HEADERS += desktop/ui/mlibitem.h
HEADERS += desktop/ui/mlibmodel.h
HEADERS += desktop/ui/mlibrary.h
HEADERS += desktop/ui/mplayback.h
HEADERS += desktop/ui/mtreeview.h

SOURCES += desktop/main.cpp
SOURCES += desktop/ui/mroot.cpp
SOURCES += desktop/ui/mlibitem.cpp
SOURCES += desktop/ui/mlibmodel.cpp
SOURCES += desktop/ui/mlibrary.cpp
SOURCES += desktop/ui/mplayback.cpp
SOURCES += desktop/ui/mtreeview.cpp

HEADERS += util/mtheme.h
SOURCES += util/mtheme.cpp

HEADERS += util/medialib.h
SOURCES += util/medialib.cpp

RESOURCES += desktop/mpi3media
RESOURCES += desktop.qrc


# EXTERNAL QRC BINARY RESOURCE
#rcc -binary myresource.qrc -o myresource.rcc
#QResource::registerResource("/path/to/myresource.rcc");
#rcc -no-compress myresources.qrc



#QT_DEBUG_PLUGINS = 1
#QT_FATAL_WARNINGS
#QT_QPA_PLATFORM =

# target.path = $$[QT_INSTALL_EXAMPLES]/phonon/qmusicplayer
# sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro *.png images
# sources.path = $$[QT_INSTALL_EXAMPLES]/phonon/qmusicplayer
# INSTALLS += target sources

#HEADERS += libav/include/libavcodec/avcodec.h

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libav/bin/ -lavcodec
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libav/bin/ -lavcodecd
#else:unix: LIBS += -L$$PWD/libav/bin/ -lavcodec

#INCLUDEPATH += $$PWD/libav/include
#DEPENDPATH += $$PWD/libav/include
