#!/bin/sh
set -e

# Configure Build
. ./scripts/config.sh

# Create Sysroot
mkdir -p $SYSROOT
mkdir -p $SYSROOT$BOOT_DIR
mkdir -p $SYSROOT$KERNEL_DIR
mkdir -p $SYSROOT$USR_INCLUDE_DIR
mkdir -p $SYSROOT$USR_LIB_DIR
mkdir -p $SYSROOT$BIN_DIR
mkdir -p $SYSROOT$DEV_DIR

mkdir -p $BOOT_SYSROOT
mkdir -p $BOOT_SYSROOT$USR_INCLUDE_DIR
mkdir -p $BOOT_SYSROOT$USR_LIB_DIR

# Install Headers
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && $MAKE install-headers)
done

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && echo "Project: $PROJECT" && $MAKE install-bin)
done

#temp for now final image
dd if=/dev/zero of=MadOS.img bs=512 count=200
cat Boot/boot.bin Kernel/MadOS.kernel | dd of=MadOS.img conv=notrunc