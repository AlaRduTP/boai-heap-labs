#!/bin/bash

if [ $# -eq 0 ]; then
    echo "usage: $0 <target> [build-arg ...]"
    exit
fi

TARGET=$1
shift 1

ARGS=( "$@" )
BUILD_ARGS=${ARGS[@]/#/--build-arg }

docker build . \
       --build-arg CTF_TARGET=$TARGET \
       $BUILD_ARGS \
       -t boai-$TARGET