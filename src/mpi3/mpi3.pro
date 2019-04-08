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
    LIBS += -lmpi3ui -lmpi3util -lmpi3core -lmpi3assets
    LIBS += -lavcodec -lavformat -lavutil
    LIBS += -lao
}
contains(DEFINES, MPI3_BUILD_SHARED) {
    LIBS += -lmpi3ui
}


win32:contains(DEFINES, MPI3_BUILD_DESKTOP) {
    RC_FILE = assets/desktop.rc
    QMAKE_TARGET_PRODUCT = "Mpi3Desktop"
    QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"
}
