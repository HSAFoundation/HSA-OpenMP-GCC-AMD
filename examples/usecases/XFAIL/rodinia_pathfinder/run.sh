#!/bin/sh
${GCCINSTALLDIR}/bin/gcc  -lm -fopenmp -Wall -O0  -c pathfinder.cpp > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia pathfinder - Failed"
else
    echo "Rodinia pathfinder     - Missing support for globals (read only in Kernel)"
fi
