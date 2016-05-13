#!/bin/bash

HSA_RUNTIME_PATH=/opt/rocm/hsa
gcc6loc=/opt/rocm/gcc6

export LD_LIBRARY_PATH=$HSA_RUNTIME_PATH/lib:$gcc6loc/lib64

echo 
echo "Building stream"
$gcc6loc/bin/gcc -O3 -fopenmp -DSTREAM_TYPE=float -DSTREAM_ARRAY_SIZE=33554432 stream.c -o stream
echo 
echo "Building stream_tuned"
$gcc6loc/bin/gcc -O3 -fopenmp -DSTREAM_TYPE=float -DSTREAM_ARRAY_SIZE=33554432 stream.c -o stream_tuned -DTUNED -L$HSA_RUNTIME_PATH/lib -lhsa-runtime64 
echo 
echo "Running stream ..."
echo 
./stream
echo 
echo "Running stream_tuned ..."
echo 
./stream_tuned
echo 
