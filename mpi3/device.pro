target.path = /home/pi/
INSTALLS += target


QT += core
QT += gui
QT += widgets


TARGET = device
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h
