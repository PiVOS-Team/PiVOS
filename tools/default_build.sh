#!/bin/sh
set -e

DEFAULT_BUILD=.default_build

[ ! -z $1 ] && [ -d $1 ] && echo $1 > $DEFAULT_BUILD && ln -sf $(realpath $1)/launch.json .vscode/launch.json
cat $DEFAULT_BUILD
