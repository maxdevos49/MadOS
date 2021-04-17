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
export BIN="$(pwd)/.bin"
export OBJ="$(pwd)/.obj"

# Dynamic Project Paths
export SUB_PROJECT
export PROJECT_OBJ
export PROJECT_BIN
export PROJECT_PATH

# Universal Flags
export CFLAGS="-MD -Wall -Wextra -m64"
export CPPFLAGS=""
export LIBS=""

# Control Switch Defaults
export DEBUG=0
export FULL_RUN=1

# OS naming and versioning
export MAJOR_VER="0"
export MINOR_VER="1"
export BUGFIX_VER="0"

export OS_NAME="dev_OS"
export IMG_NAME="${OS_NAME}-${MAJOR_VER}_${MINOR_VER}_${BUGFIX_VER}.img"
export IMG_PATH=$BIN/$IMG_NAME

# Sub Project Names
export SUB_PROJECTS="\
    boot"
    # kernel"