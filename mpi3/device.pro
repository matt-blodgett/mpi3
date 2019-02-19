DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES += QT_DEPRECATED_WARNINGS=0x010000
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT


CONFIG += c++17


QT += core
QT += gui
QT += xml
QT += widgets


TEMPLATE = app
VERSION = 1.0.0


TARGET = mpi3
QMAKE_TARGET_PRODUCT = "Mpi3Device"
QMAKE_TARGET_DESCRIPTION = "Mpi3 Media Player"


# sudo apt-get install -y libao4 libao-dev libao-common libc6-dev libc6 libasound2 libaudio2 libpulse0
LIBS += -lao

# sudo apt-get install -y ffmpeg libavcodec57 libavdevice57 libavfilter6 libavformat57 libavutil55 libpostproc54 libswresample2 libswscale4
# sudo apt-get install -y libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libavutil-dev libpostproc-dev libswresample-dev libswscale-dev
LIBS += -lavcodec
LIBS += -lavformat
LIBS += -lavutil


target.path = /home/pi/Desktop/mpi3
INSTALLS += target


RESOURCES += assets/device.qrc


HEADERS += mglobal.h
SOURCES += mglobal.cpp

HEADERS += util/mstyle.h
SOURCES += util/mstyle.cpp
HEADERS += util/msettings.h
SOURCES += util/msettings.cpp

HEADERS += core/maudioengine.h
SOURCES += core/maudioengine.cpp
HEADERS += core/mmedialibrary.h
SOURCES += core/mmedialibrary.cpp

SOURCES += device/main.cpp
HEADERS += device/mroot.h
SOURCES += device/mroot.cpp

HEADERS += device/ui/mpanel.h
SOURCES += device/ui/mpanel.cpp
HEADERS += device/ui/mstatusbar.h
SOURCES += device/ui/mstatusbar.cpp
HEADERS += device/ui/mpanelhome.h
SOURCES += device/ui/mpanelhome.cpp
HEADERS += device/ui/mpanelmedia.h
SOURCES += device/ui/mpanelmedia.cpp
HEADERS += device/ui/mpanelplayback.h
SOURCES += device/ui/mpanelplayback.cpp
