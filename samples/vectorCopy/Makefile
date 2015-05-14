GCCHSAIL=$(GCC_HSA)/bin/gcc
LDFLAGS=-L $(GCC_HSA)/lib64 -Wl,-rpath,$(GCC_HSA)/lib64 -L$(HSA_LIBRARY_PATH) -lhsa-runtime64 -lhsakmt
CFLAGS=-lm -fopenmp -Wall -O3 

all : omp_veccopy
clean :
	rm -rf *.o omp_veccopy 
omp_veccopy.o :  omp_veccopy.c
	$(GCCHSAIL) -c $(CFLAGS) $< 
omp_veccopy : omp_veccopy.o 
	$(GCCHSAIL) omp_veccopy.o $(CFLAGS) -o $@  $(LDFLAGS) 
