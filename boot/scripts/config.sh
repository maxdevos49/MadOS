#!/bin/sh
set -e

# Sub Project Sysroot
export SYSROOT="$OBJ/$SUB_PROJECT/sysroot"
export USR_INCLUDE_DIR="usr/include"
export USR_LIB_DIR="usr/lib"

# Sub Project Libraries
export SYSTEM_LIBRARY_PROJECTS="\
    libraries/libboot"

# Sub Project Projects :D
export SYSTEM_PROJECTS="\
    $SYSTEM_LIBRARY_PROJECTS\
    legacy"
    #efi