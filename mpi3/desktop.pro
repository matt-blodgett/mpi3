QT += core
QT += xml
QT += gui
QT += widgets
QT += multimedia


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


HEADERS += util/mtheme.h
SOURCES += util/mtheme.cpp

HEADERS += util/medialib.h
SOURCES += util/medialib.cpp

HEADERS += util/maudio.h
SOURCES += util/maudio.cpp


RESOURCES += desktop/qss
RESOURCES += desktop/icons


# target.path = $$[QT_INSTALL_EXAMPLES]/phonon/qmusicplayer
# sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro *.png images
# sources.path = $$[QT_INSTALL_EXAMPLES]/phonon/qmusicplayer
# INSTALLS += target sources
