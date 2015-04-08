#!/bin/sh
./run > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia nn            - Failed"
else
    echo "Rodinia nn               - Passed"
fi
