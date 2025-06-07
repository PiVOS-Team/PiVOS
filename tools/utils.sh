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
elif [[ ${FLAGS["--release"]} ]]; then
    BUILD_TYPE=release
else
    BUILD_TYPE=$($TOOLS_PATH/default_build.sh --get)
fi

if [[ ${FLAGS["--format"]} ]]; then
    OPERATION=format
elif [[ ${FLAGS["--tidy"]} ]]; then
    OPERATION=tidy
else
    echo "Usage: ./utils.sh [--debug | --release] (--format | --tidy)" 
    exit 1
fi

ninja -C $PROJECT_PATH/build/$BUILD_TYPE clang-$OPERATION