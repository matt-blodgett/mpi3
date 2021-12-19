DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
DEFINES += QT_DEPRECATED_WARNINGS


!contains(QT, multimedia):QT += multimedia


equals(TEMPLATE, lib) {
    contains(DEFINES, MPI3_BUILD_STATIC):CONFIG += static
    contains(DEFINES, MPI3_BUILD_SHARED):CONFIG += shared
}


CONFIG += skip_target_version_ext
contains(DEFINES, MPI3_BUILD_DESKTOP):CONFIG += c++1z
contains(DEFINES, MPI3_BUILD_DEVICE):CONFIG += c++14


LIBS += "-L$${DESTDIR}"
unix:contains(DEFINES, MPI3_BUILD_SHARED) {
    QMAKE_RPATHDIR += $$DESTDIR
}


BUILD_TARGET = mpi3$${BUILD_TARGET}
win32:BUILD_VERSION = $${BUILD_VERSION}.0
TARGET = $$BUILD_TARGET
VERSION = $$BUILD_VERSION


!build_pass:message("BUILD: $$TEMPLATE $$TARGET v$$VERSION")
