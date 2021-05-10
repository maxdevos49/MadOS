#!/bin/sh
set -e

# . ./scripts/config.sh

# . ./scripts/clean.sh

. ./scripts/build.sh

echo "${GREEN}Running devOS:${NC}"

# https://qemu-project.gitlab.io/qemu/
qemu-system-x86_64                                                                 \
    -machine q35                                                                   \
    -serial stdio                                                                  \
    -boot menu=on,splash-time=6000                                                 \
    -drive media=disk,format=raw,file=$IMG_PATH                                    
    # -no-shutdown                                                                   \
    # -no-reboot                                                                     \
    # -device nec-usb-xhci,id=xhci                                                   \
    # -s -S                                                                          \

# Useful Qemu Console Commands 
##############################
# Console Entry: 
#   Ctrl + Alt + 2
#
# Save Physical Memory:
#   pmemsave <start address> <length> <filename>
