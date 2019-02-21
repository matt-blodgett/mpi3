if(!contains(DEFINES, MPI3_BUILD_DESKTOP):!contains(DEFINES, MPI3_BUILD_DEVICE)) {
    error("define either MPI3_BUILD_DESKTOP or MPI3_BUILD_DEVICE when building")
}
if(contains(DEFINES, MPI3_BUILD_DESKTOP):contains(DEFINES, MPI3_BUILD_DEVICE)) {
    error("define only one of MPI3_BUILD_DESKTOP or MPI3_BUILD_DEVICE when building")
}

contains(DEFINES, MPI3_BUILD_DESKTOP):BUILD_TARGET = desktop
contains(DEFINES, MPI3_BUILD_DEVICE):BUILD_TARGET = device


message("build.pro")
message("Build Target = $$BUILD_TARGET")
message("Build File = $$_PRO_FILE_")
message("Build Dest Dir = $$DESTDIR")
message("Qt Version = $$[QT_VERSION]")
message("Qt Bin Dir = $$[QT_INSTALL_BINS]")
message("Qt Docs Dir = $$[QT_INSTALL_DOCS]")
message("Qt Libs Dir = $$[QT_INSTALL_LIBS]")
message("Qt Headers Dir = $$[QT_INSTALL_HEADERS]")
message("qmake = $$QMAKE_QMAKE")
message("qmakespec = $$QMAKESPEC")
message("Host OS = $$QMAKE_HOST.os v$$QMAKE_HOST.version")
message("Host Name = $$QMAKE_HOST.name")
message("Host Arch = $$QMAKE_HOST.arch")
message("Host Cores = $$QMAKE_HOST.cpu_count")


TEMPLATE = subdirs
SUBDIRS += $$PWD/mpi3/core
SUBDIRS += $$PWD/mpi3/utils
SUBDIRS += $$PWD/mpi3/$$BUILD_TARGET
SUBDIRS += $$PWD/mpi3
CONFIG += ordered
