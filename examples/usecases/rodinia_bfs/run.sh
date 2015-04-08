#!/bin/sh
if [ -f result.txt ]
then
	rm result.txt
fi

./run >> /dev/null

diff --brief result.txt golden_output >/dev/null
comp_value=$?

if [ $comp_value -eq 0 ]
then
    echo "Rodinia BFS              - Passed"
else
    echo "Rodinia BFS              - Failed"
fi
