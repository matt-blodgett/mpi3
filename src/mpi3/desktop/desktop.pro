include(desktop.pri)


TEMPLATE = lib
BUILD_TARGET = ui
BUILD_VERSION = 1.0.0
include(../common.pri)


contains(DEFINES, MPI3_BUILD_SHARED) {
    DEFINES += MPI3_SHARED_LIBRARY_UI
    LIBS += -lmpi3core -lmpi3utils
}
