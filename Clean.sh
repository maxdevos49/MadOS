#!/bin/sh
set -e

. ./Config.sh

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && $MAKE clean)
done

rm -rf $SYSROOT