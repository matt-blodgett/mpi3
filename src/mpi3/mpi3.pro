QT += xml
QT += widgets


TEMPLATE = app
contains(DEFINES, MPI3_BUILD_DESKTOP) {
    BUILD_TARGET = desktop
    BUILD_VERSION = 1.0.0
}
contains(DEFINES, MPI3_BUILD_DEVICE) {
    BUILD_TARGET = device
    BUILD_VERSION = 1.0.0
}

include(common.pri)


LIBS += "-L$$OUT_PWD"
LIBS += -lmpi3ui
LIBS += -lmpi3core
LIBS += -lmpi3utils

LIBS += -lao
LIBS += -lavcodec
LIBS += -lavformat
LIBS += -lavutil

INCLUDEPATH += core
INCLUDEPATH += utils
INCLUDEPATH += $$BUILD_TARGET

DEFINES += MPI3_APP_NAME=\\\"$${TARGET}\\\"
SOURCES += main.cpp


RESOURCES += assets/fonts.qrc
contains(DEFINES, MPI3_BUILD_DESKTOP) {
    RESOURCES += assets/desktop.qrc
    RC_FILE = assets/desktop.rc
    win32:QMAKE_TARGET_PRODUCT = "Mpi3Desktop"
    win32:QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"
}
contains(DEFINES, MPI3_BUILD_DEVICE) {
    RESOURCES += assets/device.qrc
}
