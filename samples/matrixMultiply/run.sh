#!/bin/sh
export LD_LIBRARY_PATH=/opt/rocm/hsa/lib:/opt/rocm/gcc6/lib64
./matmul > test.output
diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Matrix multiplication    - Failed"
else
    echo "Matrix multiplication    - Passed"
fi
