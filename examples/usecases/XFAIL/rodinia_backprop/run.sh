#!/bin/sh
${GCCINSTALLDIR}/bin/gcc -lm -fopenmp -Wall -O2 backprop.c -c > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia backprop - Failed"
else
    echo "Rodinia backprop       - Call to built-in exp()"
fi
