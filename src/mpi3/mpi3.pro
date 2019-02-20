RESOURCES += assets/fonts.qrc


TEMPLATE = app
contains(DEFINES, MPI3_BUILD_DESKTOP) {
    BUILD_TARGET = desktop
    BUILD_VERSION = 1.0.0
}
contains(DEFINES, MPI3_BUILD_DEVICE) {
    BUILD_TARGET = device
    BUILD_VERSION = 1.0.0
}

include(common.pri)


QT += xml
QT += widgets


contains(DEFINES, MPI3_BUILD_DESKTOP) {
    RESOURCES += assets/desktop.qrc
    RC_FILE = assets/desktop.rc

    win32 {
        QMAKE_TARGET_PRODUCT = "Mpi3Desktop"
        QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"

        LIBS += $$PWD/lib/libao/libao.dll.a
        LIBS += $$PWD/lib/ffmpeg/avcodec.lib
        LIBS += $$PWD/lib/ffmpeg/avformat.lib
        LIBS += $$PWD/lib/ffmpeg/avutil.lib

        INCLUDEPATH += $$PWD/lib/libao
        DEPENDPATH += $$PWD/lib/libao

        INCLUDEPATH += $$PWD/lib/ffmpeg
        DEPENDPATH += $$PWD/lib/ffmpeg
    }
    unix {
        LIBS += libmpi3ui.a
        LIBS += libmpi3core.a
        LIBS += libmpi3utils.a
    }

}
contains(DEFINES, MPI3_BUILD_DEVICE) {
    RESOURCES += assets/device.qrc

    win32 {
        error("cannot build device app on windows!")
    }
    unix {
        LIBS += libmpi3ui.a
        LIBS += libmpi3core.a
        LIBS += libmpi3utils.a
    }
}


LIBS += -lao
LIBS += -lavcodec
LIBS += -lavformat
LIBS += -lavutil

INCLUDEPATH += core
INCLUDEPATH += utils
INCLUDEPATH += $$BUILD_TARGET

DEFINES += MPI3_APP_NAME=\\\"$${TARGET}\\\"
SOURCES += main.cpp
