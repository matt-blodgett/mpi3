QT += core
QT += gui
QT += widgets


TARGET = desktop
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000


SOURCES += main.cpp
SOURCES += mainwindow.cpp


HEADERS += mainwindow.h
