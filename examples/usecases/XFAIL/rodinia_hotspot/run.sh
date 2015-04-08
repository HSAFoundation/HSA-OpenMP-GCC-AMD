#!/bin/sh
${GCCINSTALLDIR}/bin/gcc  -g -fopenmp -O2 hotspot_openmp.cpp -c > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia hotspot - Failed"
else
    echo "Rodinia hotspot        - Missing support for globals (read only in Kernel)"
fi
