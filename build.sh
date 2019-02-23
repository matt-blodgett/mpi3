#!/usr/bin/env bash


function show_message() {
    echo -ne "\e[1m\e[36m"
    echo "$1" && echo -ne "\e[0m"
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


PRO_FILE="$PWD/src/build.pro"


cd build
check_error "$?"

source config.sh
check_error "$?"

QMAKE_ARGS=()
while IFS='' read -r line || [[ -n "$line" ]]; do
    QMAKE_ARGS+=( "$line" )
done < config.txt
check_error "$?"


BUILD_DIR="build-$BUILD_TARGET-$BUILD_MODE"
rm -rf "$BUILD_DIR" && mkdir "$BUILD_DIR" && cd "$BUILD_DIR"


declare -A QMAKE_DEFINES=(
    ["device"]="MPI3_BUILD_DEVICE"
    ["desktop"]="MPI3_BUILD_DESKTOP"
    ["static"]="MPI3_BUILD_STATIC"
    ["shared"]="MPI3_BUILD_SHARED"
)

QMAKE_ARGS+=( "LIBS+=$PWD/mpi3" )
QMAKE_ARGS+=( "DESTDIR=$PWD/mpi3" )
QMAKE_ARGS+=( "CONFIG+=$BUILD_MODE" )
QMAKE_ARGS+=( "DEFINES+=${QMAKE_DEFINES[$BUILD_TARGET]}" )
QMAKE_ARGS+=( "DEFINES+=${QMAKE_DEFINES[$BUILD_LIBS]}" )


MAKE_ARGS=()
MAKE_ARGS+=( "-j$MAKE_JOBS" )
if [[ "$BUILD_TARGET" == "device" && "$INSTALL_DEST" ]]; then
    MAKE_ARGS+=( "--ignore-errors" "--keep-going" )
fi


"$BUILD_QMAKE" "$PRO_FILE" -spec "$QMAKE_SPEC" "${QMAKE_ARGS[@]}"
"$BUILD_MAKE" qmake_all
"$BUILD_MAKE" -f "$PWD/Makefile" qmake_all
"$BUILD_MAKE" "${MAKE_ARGS[@]}"
"$BUILD_MAKE" install


if [[ "$BUILD_TARGET" == "device" && "$INSTALL_DEST" && "$INSTALL_HOST" ]]; then
    INSTALL_ARGS=( "-o" "./mpi3device" "main.o" "qrc_fonts.o" "qrc_device.o" )
    INSTALL_ARGS+=( "libmpi3ui.a" "libmpi3core.a" "libmpi3utils.a" )
    INSTALL_ARGS+=( "-lao" "-lavcodec" "-lavformat" "-lavutil" )
    INSTALL_ARGS+=( "-L/usr/local/qt5pi/lib" "-lQt5Widgets" "-lQt5Gui" "-lQt5Xml" "-lQt5Core" )
    INSTALL_ARGS+=( "-L=/opt/vc/lib" "-lGLESv2" "-lpthread" )

    scp -r "$PWD" "$INSTALL_HOST:$INSTALL_DEST"
    ssh "$INSTALL_HOST" "cd $INSTALL_DEST/$BUILD_DIR/mpi3 && echo \$PWD && g++ ${INSTALL_ARGS[@]} -v"
fi
