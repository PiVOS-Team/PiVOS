#!/bin/sh
set -e

meson compile -C $(./tools/default_build.sh) kernel tools "$@"
