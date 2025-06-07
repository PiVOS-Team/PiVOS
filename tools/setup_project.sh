#!/bin/bash
TOOLS_PATH=$(dirname "$(readlink -f "$0")")
PROJECT_PATH=$(dirname $TOOLS_PATH)

source $TOOLS_PATH/parser.sh

declare -A FLAGS=()
declare -A OPTS=()

parse_args FLAGS OPTS "$@"

if [[ ${FLAGS["--reconfigure"]} ]]; then
    ADDITIONAL_OPTIONS="--reconfigure"
fi

meson setup --cross-file aarch64-linux-gnu.txt $PROJECT_PATH/build/release $PROJECT_PATH/pivos --buildtype=release $ADDITIONAL_OPTIONS
meson setup --cross-file aarch64-linux-gnu.txt $PROJECT_PATH/build/debug $PROJECT_PATH/pivos --buildtype=debug $ADDITIONAL_OPTIONS
meson setup --cross-file aarch64-linux-gnu.txt $PROJECT_PATH/build/misc $PROJECT_PATH/pivos $ADDITIONAL_OPTIONS

meson compile -C $PROJECT_PATH/build/misc build_tools
$TOOLS_PATH/default_build.sh --set --debug