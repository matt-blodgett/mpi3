QT += gui
QT += core
QT += widgets
QT += xml


TARGET = desktop


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


HEADERS += desktop/ui/mroot.h
HEADERS += desktop/ui/mpanels.h
HEADERS += desktop/ui/mplayback.h
HEADERS += desktop/ui/mtrees.h


SOURCES += desktop/main.cpp
SOURCES += desktop/ui/mroot.cpp
SOURCES += desktop/ui/mpanels.cpp
SOURCES += desktop/ui/mplayback.cpp
SOURCES += desktop/ui/mtrees.cpp


HEADERS += util/medialib.h
SOURCES += util/medialib.cpp


RESOURCES += desktop/qss
RESOURCES += desktop/icons
