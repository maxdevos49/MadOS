#!/bin/sh
set -e

. ./scripts/config.sh

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && $MAKE clean)
done

rm -rf $SYSROOT
rm -f MadOS.bin MadOS.img