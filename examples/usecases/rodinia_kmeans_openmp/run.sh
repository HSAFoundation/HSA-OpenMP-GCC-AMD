#!/bin/sh
./kmeans -i 100 -n 256 |grep -v Time > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia kmeans           - Failed"
else
    echo "Rodinia kmeans           - Passed"
fi
