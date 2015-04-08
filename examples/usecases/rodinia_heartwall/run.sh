#!/bin/sh
#${HSAILASM_095} -disassemble hsakernel.o  > test.output 2>& 1

./run > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia heartwall        - Failed"
else
    echo "Rodinia heartwall        - Passed"
fi

