RESOURCES += fonts.qrc

contains(DEFINES, MPI3_BUILD_DESKTOP):RESOURCES += desktop.qrc
contains(DEFINES, MPI3_BUILD_DEVICE):RESOURCES += device.qrc
