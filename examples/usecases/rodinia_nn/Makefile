GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 -lhsakmt
CFLAGS=-lm -fopenmp -Wall  $(OPTFLAGS)
#CFLAGS=-lm -fopenmp -Wall -O0
#CFLAGS=-lm -fopenmp -Wall -O3

all : nn_hsa

clean :
	rm -rf *.o nn_hsa

# GCC Specifics
nn_hsa.o :  nn_hsa.c
	$(GCCHSAIL) -c $(CFLAGS) $< 
nn_hsa : nn_hsa.o 
	$(GCCHSAIL)  nn_hsa.o $(CFLAGS) -o $@  $(LDFLAGS)

