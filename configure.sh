#!/usr/bin/env bash


function show_usage() {
    cat <<EOF
Usage: configure.sh [OPTIONS]
Build script for the mpi3 project

Options:
 --help             display this message and exit

Targets:
 --desktop          build desktop app
 --device           build device app

Libraries:
 --static           build static libraries
 --shared           build shared libraries

Modes:
 --release          build in release mode
 --debug            build in debug mode

Variables:
 --qmake-path       path to qmake
 --qmake-spec       qmakespec value for qmake
 --qmake-arg        add additional qmake args
 --make-path        path to make
 --make-jobs        make jobs

Install:
 --install-dest     install destination directory
 --install-host     install host device [host@address]

Git: <https://github.com/matt-blodgett/mpi3.git>
EOF
}


function show_error() {
    echo -ne "\e[1m\e[31m"
    echo "$1" && echo -ne "\e[0m"
    if [[ "$2" ]]; then exit "$2"; else exit 1; fi
}


function check_error() {
    if [[ "$?" != 0 ]]; then
        show_error "the program exited with exit code $?" "$?"
    fi
}


if [[ "$#" == 0 ]]; then
    show_usage && exit 1
fi


BUILD_TARGET=
BUILD_LIBS=
BUILD_MODE=
BUILD_QMAKE=
QMAKE_SPEC=
QMAKE_ARGS=()
BUILD_MAKE=
MAKE_JOBS=
INSTALL_DEST=
INSTALL_HOST=


while [[ $# -gt 0 ]]; do
    case "$1" in
        --help    ) show_usage; exit 0; ;;
        --desktop ) BUILD_TARGET="desktop"; ;;
        --device  ) BUILD_TARGET="device"; ;;
        --static  ) BUILD_LIBS="static"; ;;
        --shared  ) BUILD_LIBS="shared"; ;;
        --debug   ) BUILD_MODE="debug"; ;;
        --release ) BUILD_MODE="release"; ;;
        --qmake-path ) BUILD_QMAKE="$2"; shift; ;;
        --qmake-spec ) QMAKE_SPEC="$2"; shift; ;;
        --qmake-arg  ) QMAKE_ARGS+=( "$2" ); shift; ;;
        --make-path  ) BUILD_MAKE="$2"; shift; ;;
        --make-jobs  ) MAKE_JOBS="$2"; shift; ;;
        --install-dest ) INSTALL_DEST="$2"; shift; ;;
        --install-host ) INSTALL_HOST="$2"; shift; ;;
        * ) show_error "unrecognized flag '$1'"; ;;
    esac
    shift
done


if [[ ! "$BUILD_TARGET" ]]; then show_error "missing required build target [--desktop|--device]"; fi
if [[ ! "$BUILD_LIBS" ]]; then show_error "missing required build target [--static|--shared]"; fi
if [[ ! "$BUILD_MODE" ]]; then show_error "missing required build mode [--release|--debug]"; fi
if [[ ! "$BUILD_QMAKE" ]]; then show_error "missing required qmake path [--qmake-path]"; fi
if [[ ! "$QMAKE_SPEC" ]]; then show_error "missing required qmakespec [--qmake-spec]"; fi
if [[ ! "$BUILD_MAKE" ]]; then show_error "missing required make path [--make-path]"; fi
if [[ ! "$MAKE_JOBS" ]]; then MAKE_JOBS="$(nproc --all)"; fi


if [[ -d build ]]; then rm -rf build; fi
mkdir build && cd build
check_error "$?"


CONFIG_FILE="config.sh"
CONFIG_VARS=$(cat <<EOF
BUILD_TARGET="$BUILD_TARGET"
BUILD_LIBS="$BUILD_LIBS"
BUILD_MODE="$BUILD_MODE"
BUILD_QMAKE="$BUILD_QMAKE"
QMAKE_SPEC="$QMAKE_SPEC"
BUILD_MAKE="$BUILD_MAKE"
MAKE_JOBS="$MAKE_JOBS"
INSTALL_DEST="$INSTALL_DEST"
INSTALL_HOST="$INSTALL_HOST"
EOF
)

echo "$CONFIG_VARS" > "$CONFIG_FILE"


CONFIG_FILE="config.txt"
touch "$CONFIG_FILE"
for ARG in "${QMAKE_ARGS[@]}"; do
    echo "$ARG" >> "$CONFIG_FILE"
done
