#!/bin/sh
HSA_RUNTIME_PATH=/opt/hsa
#  /opt/hsa/lib also needs Okra lib
HSA_LIBRARY_PATH=$HSA_RUNTIME_PATH/lib:$LD_LIBRARY_PATH
export GCCINSTALLDIR=/usr/local/hsagcc
export OPTFLAGS=-O3
export LD_LIBRARY_PATH=/usr/local/lib:$HSA_LIBRARY_PATH:$GCCINSTALLDIR/lib64:$GCCINSTALLDIR/lib32:$LD_LIBRARY_PATH
./run > temp
grep -e Total -e Time -e [fF]ail -e [eE]rror -e [uU]nable temp > test.output

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia srad v1          - Failed"
else
    echo "Rodinia srad v1          - Passed"
fi
