#!/bin/bash
#
export FILE_NAME="HelloGRSU.c"
export FILE_NAME_OUT="HelloGRSUexe"

export MPICH2_ROOT="/opt/mpich2/gnu"
export PATH="$MPICH2_ROOT/bin:$PATH"

$MPICH2_ROOT/bin/mpicc  $FILE_NAME -o $FILE_NAME_OUT 

exit 0