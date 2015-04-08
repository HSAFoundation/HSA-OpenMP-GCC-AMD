. ./setenv.gcc
echo "*****************************************"
echo "OpenMP 3 Compile time + Runtime testcases"
echo "OpenMP 4 testcases coming soon           "
echo "*****************************************"
. ./setenv.gcc
cd ./usecases/rodinia_nn
./run.sh
cd ../../usecases/rodinia_nw
./run.sh
cd ../../usecases/rodinia_lud/omp
./run.sh
#   Skilling srad because it crashes HSA 1.0P
#cd ../../../usecases/rodinia_srad/srad_v1
#./run.sh
#cd ../../../usecases/rodinia_srad/srad_v2
#./run.sh
cd ../../../usecases/rodinia_bfs
./run.sh
#cd ../../usecases/rodinia_heartwall
#./run.sh
cd ../../usecases/rodinia_kmeans_openmp
./run.sh
cd ../../usecases/matmul
./run.sh
cd ../../usecases/fortran_veccopy
./run.sh
cd ../../usecases/c_veccopy
./run.sh
cd ../../usecases/c_veccopy_lto
./run.sh
cd ../../usecases/c_veccopy_multiple_brig
./run.sh
echo""
echo "Following tests are expected to fail(Compile time testcases)"
echo "************************************************************"
cd ../../usecases/XFAIL/rodinia_leukocyte/OpenMP
./run.sh
cd ../../../../usecases/XFAIL/rodinia_backprop
./run.sh
cd ../../../usecases/XFAIL/rodinia_myocyte
./run.sh
cd ../../../usecases/XFAIL/rodinia_pathfinder
./run.sh
cd ../../../usecases/XFAIL/rodinia_hotspot
./run.sh
cd ../../../usecases/XFAIL/rodinia_steamcluster
./run.sh
cd ../../../usecases/XFAIL/rodinia_particlefilter
./run.sh
#  This test needs work 
#cd ../../../usecases/XFAIL/rodinia_b+tree
#./run.sh
cd ../../../usecases/XFAIL/GCC_unittest
./run.sh
