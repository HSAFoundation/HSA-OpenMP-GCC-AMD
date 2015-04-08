#! /bin/sh
. ./setenv.gcc
for i in `ls usecases`
do
	if [ "$i" = "XFAIL" ]; then
		continue
	fi
	echo "Building $i"
	cd usecases/$i
	make clean
	make
	cd ../..
done
