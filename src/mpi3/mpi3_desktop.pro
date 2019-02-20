RESOURCES += assets/fonts.qrc
RESOURCES += assets/desktop.qrc
RC_FILE = assets/desktop.rc

TEMPLATE = app
BUILD_TARGET = desktop
BUILD_VERSION = 1.0.0
include(common.pri)

DEFINES *= MPI3_BUILD_DESKTOP
include(mpi3.pri)
