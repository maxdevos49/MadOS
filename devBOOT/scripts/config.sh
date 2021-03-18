#!/bin/sh
set -e

export RED='\033[0;31m'
export GREEN='\033[0;32m'
export NC='\033[0m' # No Color

export CC=x86_64-elf-gcc
export AR=x86_64-elf-ar
export LD=x86_64-elf-ld
export OBJCOPY=x86_64-elf-objcopy
export NASM=nasm
export MAKE="make -j4"

export SYSROOT="$(pwd)/sysroot"

export BOOT_DIR="/boot"
export USR_INCLUDE_DIR="/usr/include"
export USR_LIB_DIR="/usr/lib"

export CFLAGS="-MD -Wall -Wextra -m64"
export CPPFLAGS=""
export LIBS="-nostdlib -lgcc"

export SYSTEM_LIBRARY_PROJECTS="libraries/libboot"
export SYSTEM_PROJECTS="$SYSTEM_LIBRARY_PROJECTS legacy"
# export SYSTEM_PROJECTS="$SYSTEM_LIBRARY_PROJECTS legacy efi" # Future