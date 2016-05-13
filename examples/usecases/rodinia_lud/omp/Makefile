GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 
CFLAGS=-lm -fopenmp -Wall -O3  -I../common

all : lud_omp
clean :
	rm -rf *.o lud_omp
lud.o :  lud.c 
	$(GCCHSAIL) -c $(CFLAGS) $< 
lud_omp.o :  lud_omp.c 
	$(GCCHSAIL) -c $(CFLAGS) $< 
lud_omp: lud_omp.o ../common/common.o  lud.o
	$(GCCHSAIL) lud_omp.o lud.o ../common/common.o $(CFLAGS) -o $@  $(LDFLAGS) 
