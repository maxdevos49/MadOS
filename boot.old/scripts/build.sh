#!/bin/sh
set -e

# Configure Build
. ./scripts/config.sh

# Create sub project intermediate Folder
mkdir -p $OBJ/$SUB_PROJECT

# Create Sub Project System Root
mkdir -p $SYSROOT
mkdir -p $SYSROOT/$USR_INCLUDE_DIR
mkdir -p $SYSROOT/$USR_LIB_DIR

# Install Headers to System Root
echo "${GREEN_COLOR}Installing Project Headers: ${RESET_COLOR}"

for PROJECT in $SYSTEM_PROJECTS; do
    echo "${RED_COLOR}Project: $PROJECT${RESET_COLOR}"

    (cd $PROJECT && rsync -avm --include='*.h' -f 'hide,! */' .  $SYSROOT/$USR_INCLUDE_DIR)
done

# Compile Projects
echo "${GREEN_COLOR}Building and Installing Project Bins: ${RESET_COLOR}"

for PROJECT in $SYSTEM_PROJECTS; do
    echo "${RED_COLOR}Project: $PROJECT${RESET_COLOR}"

    # Set Dynamic Paths
    PROJECT_OBJ=$OBJ/$SUB_PROJECT/$PROJECT
    PROJECT_BIN=$BIN/$SUB_PROJECT/$PROJECT
    PROJECT_PATH=$(pwd)/$PROJECT

    # gcc does not seem to be able to generate supporting directories
    mkdir -p $PROJECT_OBJ
    rsync -a $PROJECT_PATH/ $PROJECT_OBJ/ --include \*/ --exclude \*

    # Install binaries
    (cd $PROJECT && $MAKE install-bin)
done
