#!/bin/sh
set -e

# Configure Build
. ./scripts/config.sh

# Create Sysroot
mkdir -p $SYSROOT
mkdir -p $SYSROOT$BOOT_DIR
mkdir -p $SYSROOT$USR_INCLUDE_DIR
mkdir -p $SYSROOT$USR_LIB_DIR

# Install Headers
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && $MAKE install-headers)
done

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && echo "Project: $PROJECT" && $MAKE install-bin)
done