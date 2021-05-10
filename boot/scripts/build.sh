#!/bin/sh
set -e

DIR=$(pwd)/limine

if [[ ! -d $DIR ]]
then
    # Download the latest Limine binary release.
    git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1

    # Build limine-install.
    make -C limine
fi

# Create a directory which will be our ISO root.
if [[ ! -d $ISO_ROOT ]]
then
    mkdir -p $ISO_ROOT
fi

# Copy the relevant files over.
cp -v \
    limine.cfg\
    limine/limine.sys\
    limine/limine-cd.bin\
    limine/limine-eltorito-efi.bin\
    $ISO_ROOT/

# Create the bootable ISO.
xorriso -as mkisofs\
    -b limine-cd.bin\
    -no-emul-boot\
    -boot-load-size 4\
    -boot-info-table \
    -eltorito-alt-boot\
    -e limine-eltorito-efi.bin\
    -no-emul-boot $ISO_ROOT -o $IMG_PATH