#!/bin/bash
TOOLS_PATH=$(dirname "$(readlink -f "$0")")
PROJECT_PATH=$(dirname $TOOLS_PATH)
CLEAN=false

source $PROJECT_PATH/build/misc/.env
source $TOOLS_PATH/parser.sh

declare -A FLAGS=()
declare -A OPTS=()

parse_args FLAGS OPTS "$@"

if [[ ${FLAGS["--debug"]} ]]; then
    BUILD_TYPE=debug
elif [[ ${FLAGS["--release"]} ]]; then
    BUILD_TYPE=release
else
    BUILD_TYPE=$($TOOLS_PATH/default_build.sh --get)
fi

if [[ ${FLAGS["--clean"]} ]]; then
    ADDITIONAL_OPTIONS="--clean"
fi

meson compile -C $PROJECT_PATH/build/$BUILD_TYPE build_sources $ADDITIONAL_OPTIONS