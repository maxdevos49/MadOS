#!/bin/sh
set -e

# Miscellaneous
export RED_COLOR='\033[0;31m'
export GREEN_COLOR='\033[0;32m'
export RESET_COLOR='\033[0m'

# Alias's for easier portability in the future
export CC=x86_64-elf-gcc
export AR=x86_64-elf-ar
export LD=x86_64-elf-ld
export OBJCOPY=x86_64-elf-objcopy
export NASM=nasm
export MAKE=make

# Output Directories
export BIN="$(pwd)/~bin"
export OBJ="$(pwd)/~obj"
export ISO="$(pwd)/~iso"

# Dynamic Project Paths
export SUB_PROJECT
export PROJECT_OBJ
export PROJECT_BIN
export PROJECT_PATH

# Universal Flags
export CFLAGS=""
export CPPFLAGS=""
export LDFLAGS=""
export LIBS=""

# Control Switch Defaults
export DEBUG=0
export FULL_RUN=1

# OS naming and versioning
export MAJOR_VER="0"
export MINOR_VER="1"
export BUGFIX_VER="0"

export OS_NAME="dev_OS"
export IMG_NAME="${OS_NAME}-${MAJOR_VER}_${MINOR_VER}_${BUGFIX_VER}.iso"
export IMG_PATH=$ISO/$IMG_NAME

export ISO_ROOT=$ISO/iso_root
export SYSROOT=$ISO_ROOT
export KERNEL_DIR="/kernel"
export USR_LIB_DIR="/usr/lib"
export USR_INCLUDE_DIR="/usr/include"


# Sub Project Names
export SUB_PROJECTS="\
    kernel\
    boot"