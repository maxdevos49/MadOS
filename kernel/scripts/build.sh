#!/bin/sh
set -e

# Configure Build
. ./scripts/config.sh

# # Create Sysroot
mkdir -p $SYSROOT/kernel
mkdir -p $SYSROOT/usr/include
mkdir -p $SYSROOT/usr/lib

# Install Headers to System Root
echo "${GREEN_COLOR}Installing Project Headers: ${RESET_COLOR}"

# Install Headers
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && $MAKE install-headers)
done

# Compile Projects
echo "${GREEN_COLOR}Building and Installing Project Binaries: ${RESET_COLOR}"

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    echo "${RED_COLOR}Project: $PROJECT${RESET_COLOR}"

    # Set Dynamic Paths
    PROJECT_OBJ=$OBJ/$SUB_PROJECT/$PROJECT
    PROJECT_BIN=$BIN/$SUB_PROJECT/$PROJECT
    PROJECT_PATH=$(pwd)/$PROJECT

    # gcc does not seem to be able to generate supporting directories
    mkdir -p $PROJECT_OBJ
    rsync -a $PROJECT_PATH/ $PROJECT_OBJ/ --include \*/ --exclude \*

    (cd $PROJECT && $MAKE install-bin)
done