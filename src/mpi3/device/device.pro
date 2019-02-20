include(ui/ui.pri)
include(device.pri)

QT += widgets

INCLUDEPATH += ../core
INCLUDEPATH += ../utils

TEMPLATE = lib
CONFIG += staticlib
BUILD_TARGET = ui
BUILD_VERSION = 1.0.0
include(../common.pri)
