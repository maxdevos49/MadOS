#!/bin/sh
set -e

. ./scripts/config.sh

. ./scripts/build.sh

# qemu-system-x86_64 -no-reboot -no-shutdown -drive format=raw,media=disk,file=MadOS.img -device ahci,id=ahci
# qemu-system-x86_64 -drive format=raw,media=disk,file=MadOS.img -device ahci,id=ahci

# TODO make something happen here


# -drive format=raw,media=disk,file=MadOS.img