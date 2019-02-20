#!/usr/bin/env bash


function print_usage() {
    cat <<EOF
Usage: build.sh [device|desktop] [OPTIONS]
Build script for the mpi3 project

Options:
   | --help     display this message and exit
 -b| --mode     build mode [debug,release]
 -d| --dest     destination directory
 -h| --host     device host to deploy to [host@address]
 -s| --spec     qmakespec value for qmake
 -q| --qmake    path to qmake command
 -m| --make     path to make command
 -j| --jobs     make jobs
 -w| --warn     use warn_on in qmake config

Git: <https://github.com/matt-blodgett/mpi3.git>
EOF

    if [[ ! "$1" ]]; then exit 1; else exit "$1"; fi
}


function print_error() {
    echo -ne "\e[1m"
    echo -ne "\e[31m"
    echo "$1"
    echo -ne "\e[0m"
}


function print_message() {
    echo -ne "\e[1m"
    echo -ne "\e[36m"
    echo "$1"
    echo -ne "\e[0m"
}


if [[ "$#" == 0 ]]; then print_usage 1; fi


BUILD_TARGET="$1"
BUILD_MODE=
BUILD_DEST=
BUILD_HOST=
BUILD_SPEC=
QMAKE_CMD=
MAKE_CMD="/usr/bin/make"
MAKE_JOBS="$(nproc --all)"
QMAKE_WARN="warn_off"


FLAGS_SHORT="b:d:h:s:q:m:j:w"
FLAGS_LONG="help,mode:,dest:,host:,spec:,qmake:,make:,jobs:,warn"
FLAGS=$(getopt -o "$FLAGS_SHORT" --longoptions "$FLAGS_LONG" -- "$@")
if [[ "$?" != 0 ]]; then exit "$?"; fi

eval set -- "$FLAGS"
while [[ $# -gt 0 ]]; do
    case "$1" in
           --help  ) print_usage 0 ;;
        -b|--mode  ) BUILD_MODE="$2"; shift ;;
        -d|--dest  ) BUILD_DEST="$2"; shift ;;
        -h|--host  ) BUILD_HOST="$2"; shift ;;
        -s|--spec  ) BUILD_SPEC="$2"; shift ;;
        -q|--qmake ) QMAKE_CMD="$2"; shift ;;
        -m|--make  ) MAKE_CMD="$2"; shift ;;
        -j|--jobs  ) MAKE_JOBS="$2"; shift ;;
        -w|--warn  ) QMAKE_WARN="warn_on" ;;
        -- ) break ;;
        *  ) exit 1 ;;
    esac
    shift
done


if [[ ! "$BUILD_TARGET" =~ ^(device|desktop)$ ]]; then
    print_error "invalid target '$BUILD_TARGET'; valid targets are [device,desktop]"
    exit 1
fi

if [[ ! "$BUILD_MODE" =~ ^(debug|release)$ ]]; then
    print_error "invalid mode '$BUILD_MODE' valid modes are [debug,release]"
    exit 1
fi

if [[ ! "$BUILD_SPEC" ]]; then
    print_error "missing qmakespec value [--spec]"
    exit 1
fi

if [[ ! "$QMAKE_CMD" ]]; then
    print_error "missing qmake command [--qmake]"
    exit 1
fi

exit 0
BUILD_DIR="build-$BUILD_TARGET-$BUILD_MODE"
PRO_FILE="$PWD/src/build.pro"


mkdir "$BUILD_DIR"
cd "$BUILD_DIR"


QMAKE_ARGS=()
QMAKE_ARGS+=( "DESTDIR=$PWD/mpi3" )
QMAKE_ARGS+=( "CONFIG+=$QMAKE_WARN" )
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
MAKE_ARGS+=( "-j$MAKE_JOBS" )

if [[ "$BUILD_TARGET" == "device" ]]; then
    MAKE_ARGS+=( "--ignore-errors" )
    MAKE_ARGS+=( "--keep-going" )
fi


"$QMAKE_CMD" "$PRO_FILE" -spec "$BUILD_SPEC" "${QMAKE_ARGS[@]}"

"$MAKE_CMD" qmake_all
"$MAKE_CMD" -f "$PWD/Makefile" qmake_all
"$MAKE_CMD" "${MAKE_ARGS[@]}"
"$MAKE_CMD" install


if [[ "$BUILD_TARGET" == "device" && "$BUILD_HOST" && "$BUILD_DEST" ]]; then
    BUILD_ARGS=()
    BUILD_ARGS+=( "-o" "./mpi3device" "main.o" "qrc_fonts.o" "qrc_device.o")
    BUILD_ARGS+=( "-L=/opt/vc/lib" )
    BUILD_ARGS+=( "libmpi3ui.a" "libmpi3core.a" "libmpi3utils.a" )
    BUILD_ARGS+=( "-lao" "-lavcodec" "-lavformat" "-lavutil" )
    BUILD_ARGS+=( "-L/usr/local/qt5pi/lib" "-lQt5Widgets" "-lQt5Gui" "-lQt5Xml" "-lQt5Core" )
    BUILD_ARGS+=( "-lGLESv2" "-lpthread" )

    scp -r "$PWD" "$BUILD_HOST:$BUILD_DEST"
    ssh "$BUILD_HOST" "cd $BUILD_DEST/$BUILD_DIR/mpi3 && echo \$PWD && g++ ${BUILD_ARGS[@]} -v"
fi
