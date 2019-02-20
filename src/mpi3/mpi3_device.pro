RESOURCES += assets/fonts.qrc
RESOURCES += assets/device.qrc

TEMPLATE = app
BUILD_TARGET = device
BUILD_VERSION = 1.0.0
include(common.pri)

DEFINES += MPI3_BUILD_DEVICE
include(mpi3.pri)
