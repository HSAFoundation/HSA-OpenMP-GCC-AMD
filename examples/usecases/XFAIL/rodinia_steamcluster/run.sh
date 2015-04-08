#!/bin/sh
${GCCINSTALLDIR}/bin/gcc -c -O3 -fopenmp streamcluster_omp.cpp -c > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia steamcluster- Failed"
else
    echo "Rodinia steamcluster   - Missing support for globals (read and write in Kernel)"
fi
