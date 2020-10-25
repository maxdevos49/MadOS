#!/bin/sh
set -e

# Configure Build
. ./Config.sh

# Create Sysroot
mkdir -p $SYSROOT
mkdir -p $SYSROOT/boot
mkdir -p $SYSROOT/usr/include
mkdir -p $SYSROOT/usr/lib

# Install Headers
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && $MAKE install-headers)
done

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && echo "Project: $PROJECT" && $MAKE install-bin)
done