#!/bin/sh
set -e

. ./scripts/config.sh

. ./scripts/build.sh

# qemu-system-x86_64 -d int -no-reboot -no-shutdown -drive format=raw,media=disk,file=MadOS.img -device ahci,id=ahci 
qemu-system-x86_64 -drive format=raw,media=disk,file=MadOS.img -device ahci,id=ahci 



# -drive format=raw,media=disk,file=MadOS.img 