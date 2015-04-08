#!/bin/sh
./omp_veccopy > test.output

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Vector Copy Fortran case - Failed"
else
    echo "Vector Copy Fortran case - Passed"
fi
