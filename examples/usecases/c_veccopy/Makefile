GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 
CFLAGS=-lm -fopenmp -Wall -O3 # Fail

all : omp_veccopy
clean :
	rm -rf *.o omp_veccopy 
omp_veccopy.o :  omp_veccopy.c
	$(GCCHSAIL) -c $(CFLAGS) $< 
omp_veccopy : omp_veccopy.o 
	$(GCCHSAIL) omp_veccopy.o $(CFLAGS) -o $@  $(LDFLAGS) 
