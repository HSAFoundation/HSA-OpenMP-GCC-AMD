echo "***************************************"
echo "OpenMP Compile time + Runtime testcases"
echo "***************************************"
. ./setenv.gcc
cd ./usecases/rodinia_nn
./run.sh
cd ../../usecases/rodinia_nw
./run.sh
cd ../../usecases/rodinia_lud/omp
./run.sh
cd ../../../usecases/rodinia_srad/srad_v1
./run.sh
cd ../../../usecases/rodinia_srad/srad_v2
./run.sh
cd ../../../usecases/rodinia_bfs
./run.sh
cd ../../usecases/rodinia_kmeans_openmp
./run.sh
cd ../../usecases/matmul
./run.sh
cd ../../usecases/fortran_veccopy
./run.sh
cd ../../usecases/c_veccopy
./run.sh
echo "***************************************"
