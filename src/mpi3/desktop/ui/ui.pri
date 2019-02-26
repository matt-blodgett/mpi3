include(widgets/widgets.pri)
include(models/models.pri)
include(frames/frames.pri)
include(panels/panels.pri)

INCLUDEPATH += $$PWD

HEADERS += $$PWD/mstyle.h
HEADERS += $$PWD/mactions.h
HEADERS += $$PWD/mplaybackqueue.h

SOURCES += $$PWD/mstyle.cpp
SOURCES += $$PWD/mactions.cpp
SOURCES += $$PWD/mplaybackqueue.cpp
