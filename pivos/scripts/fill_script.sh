#!/bin/sh
envsubst < $1 > $2
chmod --reference=$1 $2
chown --reference=$1 $2
