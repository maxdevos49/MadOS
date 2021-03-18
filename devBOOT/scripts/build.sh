#!/bin/sh
set -e

# Configure Build
. ./scripts/config.sh

# Create Sysroot
mkdir -p $SYSROOT
mkdir -p $SYSROOT$BOOT_DIR
mkdir -p $SYSROOT$USR_INCLUDE_DIR
mkdir -p $SYSROOT$USR_LIB_DIR

echo "${GREEN}Installing Project Headers: ${NC}"

# Install Headers
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && echo "${RED}Project: $PROJECT${NC}" && $MAKE install-headers)
done

echo "${GREEN}Building and Installing Project Bins: ${NC}"

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && echo "${RED}Project: $PROJECT${NC}" && $MAKE install-bin)
done