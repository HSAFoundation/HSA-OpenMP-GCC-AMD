#!/bin/sh
export HSA_DEBUG=1
export LD_LIBRARY_PATH=/opt/rocm/hsa/lib:/opt/rocm/gcc6/lib64
./omp_veccopy
