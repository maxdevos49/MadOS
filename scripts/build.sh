#!/bin/sh
set -e

. ./scripts/config.sh

if [ ! -d $BIN ] || [ ! -d $OBJ ]
then
    mkdir $BIN
    mkdir $OBJ
fi

# Create IMG
dd if=/dev/zero of=$IMG_PATH bs=1048576 count=128
mformat -i $IMG_PATH -v $OS_NAME -R 50 -F ::

mmd -i $IMG_PATH ::/EFI
mmd -i $IMG_PATH ::/EFI/BOOT

mmd -i $IMG_PATH ::/LEGACY
mmd -i $IMG_PATH ::/LEGACY/BOOT

for SUB_PROJECT in $SUB_PROJECTS; do
    echo "${RED_COLOR}Building Sub Project: ${SUB_PROJECT}${RESET_COLOR}"
    (cd $SUB_PROJECT && . ./scripts/build.sh)
done