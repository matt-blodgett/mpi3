if(!contains(DEFINES, MPI3_BUILD_DESKTOP):!contains(DEFINES, MPI3_BUILD_DEVICE)) {
    error("define either MPI3_BUILD_DESKTOP or MPI3_BUILD_DEVICE when building")
}
if(contains(DEFINES, MPI3_BUILD_DESKTOP):contains(DEFINES, MPI3_BUILD_DEVICE)) {
    error("define only one of MPI3_BUILD_DESKTOP or MPI3_BUILD_DEVICE when building")
}

contains(DEFINES, MPI3_BUILD_DESKTOP):BUILD_TARGET = desktop
contains(DEFINES, MPI3_BUILD_DEVICE):BUILD_TARGET = device

message(build.pro: building target $$BUILD_TARGET)


TEMPLATE = subdirs
SUBDIRS += mpi3/core
SUBDIRS += mpi3/utils
SUBDIRS += mpi3/$$BUILD_TARGET
SUBDIRS += mpi3/mpi3_$${BUILD_TARGET}.pro
CONFIG += ordered
