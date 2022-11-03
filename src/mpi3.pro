# Mpi3 Base Project
# DEFINES+=MPI3_BUILD_DESKTOP DEFINES+=MPI3_BUILD_SHARED DESTDIR=%{buildDir}/mpi3 LIBS+=-L%{buildDir}/mpi3


defineTest(mutexRequiredDefines) {
    if(!contains(DEFINES, $$1):!contains(DEFINES, $$2)) {
        error("define either $$1 or $$2 when building")
    }
    if(contains(DEFINES, $$1):contains(DEFINES, $$2)) {
        error("define only one of $$1 or $$2 when building")
    }
}


mutexRequiredDefines(MPI3_BUILD_DESKTOP, MPI3_BUILD_DEVICE)
mutexRequiredDefines(MPI3_BUILD_STATIC, MPI3_BUILD_SHARED)


contains(DEFINES, MPI3_BUILD_DESKTOP):BUILD_TARGET = desktop
contains(DEFINES, MPI3_BUILD_DEVICE):BUILD_TARGET = device


message("mpi3.pro")
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
SUBDIRS += $$PWD/mpi3/util
SUBDIRS += $$PWD/mpi3/core
SUBDIRS += $$PWD/mpi3/assets
SUBDIRS += $$PWD/mpi3/$$BUILD_TARGET
SUBDIRS += $$PWD/mpi3
CONFIG += ordered
