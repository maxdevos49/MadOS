#!/bin/sh
set -e

. ./scripts/config.sh

. ./scripts/build.sh

qemu-system-x86_64 -no-reboot -no-shutdown -drive format=raw,media=disk,file=legacy/loader -device ahci,id=ahci -serial stdio
