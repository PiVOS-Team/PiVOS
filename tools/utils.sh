#!/bin/bash
set -e

echo "Starting QEMU"
ninja -C $(./tools/default_build.sh) "$@"
