#!/bin/sh
set -e

. ./scripts/config.sh

if [ ! -d $ISO ]
then
    mkdir $ISO
fi

if [ ! -d $OBJ ]
then
    mkdir $OBJ
fi

if [ ! -d $BIN ]
then
    mkdir $BIN
fi

for SUB_PROJECT in $SUB_PROJECTS; do
    echo "${RED_COLOR}Building Sub Project: ${SUB_PROJECT}${RESET_COLOR}"
    (cd $SUB_PROJECT && . ./scripts/build.sh)
done