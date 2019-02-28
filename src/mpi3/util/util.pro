include(util.pri)


TEMPLATE = lib
BUILD_TARGET = util
BUILD_VERSION = 1.0.0
include(../common.pri)


contains(DEFINES, MPI3_BUILD_SHARED) {
    DEFINES += MPI3_SHARED_LIBRARY_UTIL
}
