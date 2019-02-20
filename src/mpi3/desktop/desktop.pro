include(ui/ui.pri)
include(ui/mvc/mvc.pri)
include(desktop.pri)

QT += widgets

INCLUDEPATH += ../core
INCLUDEPATH += ../utils

TEMPLATE = lib
CONFIG += staticlib
BUILD_TARGET = ui
BUILD_VERSION = 1.0.0
include(../common.pri)
