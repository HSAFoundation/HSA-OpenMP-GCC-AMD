GCCHSAIL=$(GCCINSTALLDIR)/bin/gfortran
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -lgfortran  -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 
CFLAGS=-lm -fopenmp -Wall -O3

all : omp_veccopy 

clean :
	rm -rf *.o omp_veccopy 

# GCC Specifics
omp_veccopy.o :  omp_veccopy.f90
	$(GCCHSAIL) -c $(CFLAGS) $<
omp_veccopy : omp_veccopy.o  
	$(GCCHSAIL) -g  omp_veccopy.o $(CFLAGS) -o $@  $(LDFLAGS)
