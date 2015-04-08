#!/bin/sh
${GCCINSTALLDIR}/bin/gcc  -lm -O3 -fopenmp ex_particle_OPENMP_seq.c -c > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia particlefilter - Failed"
else
    echo "Rodinia particlefilter - Unsupported complex atomic synchronization"
fi
