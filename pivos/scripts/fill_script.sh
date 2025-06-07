#!/bin/sh
ENV_VARS=$(printenv | cut -d= -f1 | sed 's/^/$/')
envsubst "$ENV_VARS" < $1 > $2
chmod --reference=$1 $2
chown --reference=$1 $2
