#!/bin/bash
#
export FILE_NAME="Reduce.c"
export FILE_NAME_OUT="Reduce"

export MPICH2_ROOT="/opt/mpich2/gnu"
export PATH="$MPICH2_ROOT/bin:$PATH"

$MPICH2_ROOT/bin/mpicc  $FILE_NAME -o $FILE_NAME_OUT 

exit 0