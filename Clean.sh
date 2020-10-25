#!/bin/sh
set -e

. ./Config.sh

# Compile Projects
for PROJECT in $SYSTEM_PROJECTS; do
    (cd $PROJECT && echo "Project: $PROJECT" && $MAKE clean)
done