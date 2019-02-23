include(mpi3.pri)


contains(DEFINES, MPI3_BUILD_DESKTOP) {
    BUILD_TARGET = desktop
    BUILD_VERSION = 1.0.0
}
contains(DEFINES, MPI3_BUILD_DEVICE) {
    BUILD_TARGET = device
    BUILD_VERSION = 1.0.0
}

TEMPLATE = app
include(common.pri)


contains(DEFINES, MPI3_BUILD_STATIC) {
    LIBS += -lmpi3ui -lmpi3core -lmpi3utils
    LIBS += -lavcodec -lavformat -lavutil
    LIBS += -lao
}
contains(DEFINES, MPI3_BUILD_SHARED) {
    LIBS += -lmpi3ui
}


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
