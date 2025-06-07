#!/bin/bash
TOOLS_PATH=$(dirname "$(readlink -f "$0")")
PROJECT_PATH=$(dirname $TOOLS_PATH)

source $PROJECT_PATH/build/misc/.env
source $TOOLS_PATH/parser.sh

declare -A FLAGS=()
declare -A OPTS=()

parse_args FLAGS OPTS "$@"

if [[ ${FLAGS["--set"]} ]]; then
    if [[ ${FLAGS["--debug"]} ]]; then
        echo "debug" > $PROJECT_PATH/build/.default_build
    elif [[ ${FLAGS["--release"]} ]]; then
        echo "release" > $PROJECT_PATH/build/.default_build
    fi
elif [[ ${FLAGS["--get"]} ]]; then
    cat $PROJECT_PATH/build/.default_build
fi

