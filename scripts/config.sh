#!/bin/sh
set -e

export CC=x86_64-elf-gcc
export AR=x86_64-elf-ar
export LD=x86_64-elf-ld
export OBJCOPY=x86_64-elf-objcopy
export NASM=nasm
export MAKE="make -j4"

export SYSROOT="$(pwd)/sysroot"

export BOOT_DIR="/boot"
export KERNEL_DIR="/kernel"
export DEV_DIR="/dev"
export USR_INCLUDE_DIR="/usr/include"
export USR_LIB_DIR="/usr/lib"
export BIN_DIR="/bin"

export CFLAGS="-ggdb -MD -Wall -Wextra -ffreestanding  -fstack-protector-all -mno-red-zone -m64 --sysroot=$SYSROOT -isystem=$USR_INCLUDE_DIR"
export CPPFLAGS="-D__test"
export LIBS=

export SYSTEM_LIBRARY_PROJECTS="libraries/libtest libraries/libc libraries/libgraphics"
export SYSTEM_PROJECTS="$SYSTEM_LIBRARY_PROJECTS Boot Kernel"