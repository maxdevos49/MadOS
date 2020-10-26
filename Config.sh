#!/bin/sh
set -e

export CC=x86_64-elf-gcc
export AR=x86_64-elf-ar
export LD=x86_64-elf-ld
export NASM=nasm
export MAKE="make -j4"

export SYSROOT="$(pwd)/sysroot"
export USR_INCLUDE_DIR="/usr/include"
export USR_LIB_DIR="/usr/lib"

export CFLAGS="-O0 -MD -Wall -Wextra -ffreestanding -m64 --sysroot=$SYSROOT -isystem=$USR_INCLUDE_DIR"
export CPPFLAGS="-D__test"

export SYSTEM_LIBRARY_PROJECTS="Libraries/libtest Libraries/libc"
export SYSTEM_PROJECTS="$SYSTEM_LIBRARY_PROJECTS Boot Kernel"