#!/bin/sh
./run > temp 2>& 1
grep -v "Time consumed" temp > test.output

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia LUD              - Failed"
else
    echo "Rodinia LUD              - Passed"
fi
