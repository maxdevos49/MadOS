#!/bin/sh
set -e

export SYSTEM_LIBRARY_PROJECTS="\
    libraries/libc\
    libraries/libgraphics\
    libraries/libm"

export SYSTEM_PROJECTS="\
    kernel"
    # $SYSTEM_LIBRARY_PROJECTS\