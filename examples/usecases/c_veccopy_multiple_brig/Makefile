GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 
CFLAGS=-lm -fopenmp -Wall -O3 # Pass

all : omp_veccopy 
clean :
	rm -rf *.o omp_veccopy  hsakernel.hsail
main.o :  main.c
	$(GCCHSAIL) -c $(CFLAGS) $<
veccpy1.o :  veccpy1.c
	$(GCCHSAIL) -c $(CFLAGS) $<
veccpy2.o :  veccpy2.c
	$(GCCHSAIL) -c $(CFLAGS) $<

omp_veccopy : main.o veccpy1.o veccpy2.o
	$(GCCHSAIL) main.o veccpy1.o veccpy2.o $(CFLAGS) $(LDFLAGS) -o $@
