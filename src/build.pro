BUILD_APP = device

TEMPLATE = subdirs
SUBDIRS += $$PWD/mpi3/core
SUBDIRS += $$PWD/mpi3/utils
SUBDIRS += $$PWD/mpi3/$$BUILD_APP
SUBDIRS += $$PWD/mpi3/mpi3_$${BUILD_APP}.pro
CONFIG += ordered
