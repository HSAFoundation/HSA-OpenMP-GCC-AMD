#!/bin/sh
./matmul > test.output

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Matrix multiplication    - Failed"
else
    echo "Matrix multiplication    - Passed"
fi
