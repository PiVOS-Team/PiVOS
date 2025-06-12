#!/bin/sh
set -e

meson setup --cross-file aarch64-linux-gnu.txt release pivos --buildtype=release "$@"
meson setup --cross-file aarch64-linux-gnu.txt debug pivos --buildtype=debug "$@"

meson compile -C release tools
meson compile -C debug tools

./tools/default_build.sh debug
