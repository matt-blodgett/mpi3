#!/usr/bin/env bash


#./build.sh device --mode debug --dest /home/pi/Desktop/test --host pi@192.168.0.16 --spec devices/linux-rasp-pi3-g++ --qmake /opt/qtrpi/raspi/qt5/bin/qmake
#./build.sh desktop --mode debug --spec linux-g++ --qmake /opt/qt5/5.12.0/gcc_64/bin/qmake


function usage() {
    cat <<EOF
Usage: build.sh TARGET [OPTIONS]
Build script for the mpi3 project.

Options:
   | --help     display this message and exit
 -m| --mode     build mode [debug,release]
 -d| --dest     destination directory
 -h| --host     device host to deploy to [host@address]
 -s| --spec     qmakespec value for qmake
 -q| --qmake    path to qmake command
   | --make     path to make command
 -j| --jobs     make jobs

EOF

    if [[ ! "$1" ]]; then exit 1; else exit "$1"; fi
}

if [[ "$#" == 0 ]];
    then usage 1;
fi


BUILD_TARGET="$1"
BUILD_MODE=
BUILD_DEST=
BUILD_HOST=
BUILD_SPEC=
QMAKE_CMD=
MAKE_CMD="/usr/bin/make"
MAKE_JOBS=""



FLAGS=$(getopt -o "m:d:h:s:q:j:" --longoptions "help,mode:,dest:,host:,spec:,qmake:,make:,jobs:" -- "$@")
if [[ "$?" != 0 ]]; then exit "$?"; fi

eval set -- "$FLAGS"
while [[ $# -gt 0 ]]; do
    case "$1" in
        --help     ) usage 0 ;;
        -m|--mode  ) BUILD_MODE="$2"; shift ;;
        -d|--dest  ) BUILD_DEST="$2"; shift ;;
        -h|--host  ) BUILD_HOST="$2"; shift ;;
        -s|--spec  ) BUILD_SPEC="$2"; shift ;;
        -q|--qmake ) QMAKE_CMD="$2"; shift ;;
        --make     ) MAKE_CMD="$2"; shift ;;
        -j|--jobs  ) MAKE_JOBS="$2"; shift ;;
        -- ) break ;;
        *  ) exit 1 ;;
    esac
    shift
done


if [[ ! "$BUILD_TARGET" =~ ^(device|desktop)$ ]]; then
    echo "invalid target '$BUILD_TARGET'"
    echo "valid targets are [device,desktop]"
    exit 1
fi

if [[ ! "$BUILD_MODE" =~ ^(debug|release)$ ]]; then
    echo "invalid mode '$BUILD_MODE'"
    echo "valid modes are [debug,release]"
    exit 1
fi

if [[ ! "$BUILD_SPEC" ]]; then
    echo "missing qmakespec value [--spec]"
    exit 1
fi

if [[ ! "$QMAKE_CMD" ]]; then
    echo "missing qmake command [--qmake]"
    exit 1
fi


BUILD_DIR="build-$BUILD_TARGET-$BUILD_MODE"
PRO_FILE="$PWD/src/build.pro"


mkdir "$BUILD_DIR"
cd "$BUILD_DIR"


QMAKE_ARGS=()
QMAKE_ARGS+=( "DESTDIR=$PWD/mpi3" )
QMAKE_ARGS+=( "CONFIG+=$BUILD_MODE" )

if [[ "$BUILD_MODE" == "debug" ]]; then
    QMAKE_ARGS+=( "CONFIG+=qml_debug" )
fi

if [[ "$BUILD_TARGET" == "device" ]]; then
    QMAKE_ARGS+=( "DEFINES+=MPI3_BUILD_DEVICE" )
elif [[ "$BUILD_TARGET" == "desktop" ]]; then
    QMAKE_ARGS+=( "DEFINES+=MPI3_BUILD_DESKTOP" )
fi


MAKE_ARGS=()
MAKE_ARGS+=( "-j12" )

if [[ "$BUILD_TARGET" == "device" ]]; then
    MAKE_ARGS+=( "--ignore-errors" )
    MAKE_ARGS+=( "--keep-going" )
fi


"$QMAKE_CMD" "$PRO_FILE" -spec "$BUILD_SPEC" "${QMAKE_ARGS[@]}"

"$MAKE_CMD" qmake_all
"$MAKE_CMD" -f "$PWD/Makefile" qmake_all
"$MAKE_CMD" "${MAKE_ARGS[@]}"


if [[ "$BUILD_TARGET" == "device" && "$BUILD_HOST" && "$BUILD_DEST" ]]; then
    BUILD_ARGS=()
    BUILD_ARGS+=( "-o" "./mpi3device.o" "main.o" "qrc_fonts.o" "qrc_device.o")
    BUILD_ARGS+=( "-L=/opt/vc/lib" )
    BUILD_ARGS+=( "libmpi3ui.a" "libmpi3core.a" "libmpi3utils.a" )
    BUILD_ARGS+=( "-lao" "-lavcodec" "-lavformat" "-lavutil" )
    BUILD_ARGS+=( "-L/usr/local/qt5pi/lib" "-lQt5Widgets" "-lQt5Gui" "-lQt5Xml" "-lQt5Core" )
    BUILD_ARGS+=( "-lGLESv2" "-lpthread" )

    scp -r "$PWD" "$BUILD_HOST:$BUILD_DEST"
    ssh "$BUILD_HOST" "cd $BUILD_DEST/$BUILD_DIR/mpi3 && echo \$PWD && g++ ${BUILD_ARGS[@]} -v"
fi
