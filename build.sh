#!/bin/bash -x

BIN=oclprof

rm -f ${BIN}
SRC="load.c m13100_a0-pure.c"
gcc ${SRC} -o ${BIN} -lOpenCL || exit 1
./${BIN}

