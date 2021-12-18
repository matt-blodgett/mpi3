QT += core
QT += gui
QT += sql


INCLUDEPATH += ../../

HEADERS += $$PWD/mcore.h

HEADERS += $$PWD/mglobal.h
HEADERS += $$PWD/mmedialibrary.h
HEADERS += $$PWD/mmediautil.h
HEADERS += $$PWD/maudioengine.h

SOURCES += $$PWD/mglobal.cpp
SOURCES += $$PWD/mmedialibrary.cpp
SOURCES += $$PWD/mmediautil.cpp
SOURCES += $$PWD/maudioengine.cpp
