#!/bin/sh
${GCCINSTALLDIR}/bin/gcc -lm -fopenmp -Wall -O2 main.c -c > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia myocyte - Failed"
else
    echo "Rodinia myocyte        - Call to built-in pow()"
fi
