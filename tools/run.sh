#!/bin/bash
TOOLS_PATH=$(dirname "$(readlink -f "$0")")
PROJECT_PATH=$(dirname $TOOLS_PATH)

source $PROJECT_PATH/build/misc/.env
source $TOOLS_PATH/parser.sh

declare -A FLAGS=()
declare -A OPTS=()

parse_args FLAGS OPTS "$@"

if [[ ${FLAGS["--debug"]} ]]; then
    BUILD_TYPE=debug
    
    if [[ ${FLAGS["--gdb"]} ]]; then
        DEBUG_PARAMS="-S -gdb tcp::$GDB_PORT"
    fi

elif [[ ${FLAGS["--release"]} ]]; then
    BUILD_TYPE=release
else
    BUILD_TYPE=$($TOOLS_PATH/default_build.sh --get)
fi

echo "Starting QEMU"
$QEMU_PATH -M $QEMU_BOARD -nographic -serial mon:stdio -kernel $PROJECT_PATH/build/$BUILD_TYPE/$IMG_NAME -cpu $QEMU_CPU -smp 4 -m 2G $DEBUG_PARAMS