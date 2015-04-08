#!/bin/sh
${GCCINSTALLDIR}/bin/gcc -lm -fopenmp -Wall -O3 -I../meschach_lib  track_ellipse.c -c > test.output 2>& 1

diff --brief test.output golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 1 ]
then
    echo "Rodinia Leukocyte - Failed"
else
    echo "Rodinia Leukocyte      - Call to built-in malloc()"
fi
