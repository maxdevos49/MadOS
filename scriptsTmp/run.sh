#!/bin/sh
set -e

. ./scripts/config.sh

. ./scripts/build.sh

qemu-system-x86_64 -drive format=raw,media=disk,file=MadOS.img 