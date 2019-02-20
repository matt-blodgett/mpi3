BUILD_APP = device

TEMPLATE = subdirs
SUBDIRS += mpi3/core
SUBDIRS += mpi3/utils
SUBDIRS += mpi3/$$BUILD_APP
SUBDIRS += mpi3/mpi3_$${BUILD_APP}.pro
CONFIG += ordered
