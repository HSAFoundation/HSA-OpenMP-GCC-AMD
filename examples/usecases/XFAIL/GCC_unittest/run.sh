#!/bin/sh
./run > test.output 2>& 1
TESTCASE=`ls *.c|wc -l`
diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "GCC ($TESTCASE)Negative testcases - Failed"
else
    echo "GCC                    - Verified $TESTCASE different graceful exits of un-supported features"
fi
