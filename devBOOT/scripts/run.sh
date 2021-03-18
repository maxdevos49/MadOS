#!/bin/sh
set -e

. ./scripts/config.sh

. ./scripts/build.sh

echo "${GREEN}Running devOS:${NC}"

# https://qemu-project.gitlab.io/qemu/
qemu-system-x86_64                                                                 \
    -boot menu=on,splash=devOS.jpg,splash-time=6000                                \
    -no-reboot                                                                     \
    -no-shutdown                                                                   \
    -drive id=main,format=raw,media=disk,file=legacy/legacy.img                \
    -device nec-usb-xhci,id=xhci                                                   \
    -serial stdio                                                                  \
    # -boot menu=on,splash=devOS.jpg,splash-time=6000\
    # -device usb-storage\
    # -device usb-storage,drive=xxx\
    # -drive file=/dev/sdd,if=none,id=xxx
