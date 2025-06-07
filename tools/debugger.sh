#!/bin/bash
TOOLS_PATH=$(dirname "$(readlink -f "$0")")
PROJECT_PATH=$(dirname $TOOLS_PATH)

source $PROJECT_PATH/build/misc/.env

$GDB_PATH \
    -ex "file $PROJECT_PATH/build/debug/$ELF_NAME" \
    -ex "add-symbol-file $PROJECT_PATH/build/debug/$ELF_NAME $KERNEL_PHYS_ADDR" \
    -ex "target remote $GDB_SERVER:$GDB_PORT" \
    -ex "layout asm"