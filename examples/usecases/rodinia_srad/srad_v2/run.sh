#!/bin/sh
./run > test.output

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia srad_v2          - Failed"
else
    echo "Rodinia srad_v2          - Passed"
fi
