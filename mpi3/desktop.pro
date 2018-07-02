QT += core
QT += xml
QT += gui
QT += widgets
QT += multimedia


TARGET = desktop


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


HEADERS += desktop/ui/mroot.h \
    desktop/ui/mlibitem.h \
    desktop/ui/mlibmodel.h
HEADERS += desktop/ui/mlibrary.h
HEADERS += desktop/ui/mplayback.h
HEADERS +=
HEADERS +=

SOURCES += desktop/main.cpp \
    desktop/ui/mlibitem.cpp \
    desktop/ui/mlibmodel.cpp
SOURCES += desktop/ui/mroot.cpp
SOURCES += desktop/ui/mlibrary.cpp
SOURCES += desktop/ui/mplayback.cpp
SOURCES +=
SOURCES +=


HEADERS += util/mtheme.h
SOURCES += util/mtheme.cpp

HEADERS += util/medialib.h
SOURCES += util/medialib.cpp

RESOURCES += desktop/qss
RESOURCES += desktop/icons


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
