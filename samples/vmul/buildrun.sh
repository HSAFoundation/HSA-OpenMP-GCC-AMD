#!/bin/bash

HSA_RUNTIME_PATH=/opt/rocm/hsa
gcc6loc=/opt/rocm/gcc6

export LD_LIBRARY_PATH=$HSA_RUNTIME_PATH/lib:$gcc6loc/lib64

echo 
echo "Building vmul"
$gcc6loc/bin/gcc -O3 -fopenmp  vmul.c -o vmul
echo 
echo "Running vmul..."
echo 
./vmul
