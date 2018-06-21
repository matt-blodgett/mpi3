QT += core
QT += widgets


TARGET = desktop


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


SOURCES += main.cpp
SOURCES += ui/mroot.cpp
SOURCES += ui/mpanels.cpp
SOURCES += ui/mplayback.cpp
SOURCES += ui/mtrees.cpp


HEADERS += ui/mroot.h
HEADERS += ui/mpanels.h
HEADERS += ui/mplayback.h
HEADERS += ui/mtrees.h
