GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 -lhsakmt
CFLAGS=-lm -fopenmp -Wall -O3 # Pass

all : omp_veccopy hsakernel.o
clean :
	rm -rf *.o omp_veccopy  hsakernel.hsail

omp_veccopy : main.c veccpy1.c veccpy2.c
	$(GCCHSAIL) -flto -flto-partition=none main.c veccpy1.c veccpy2.c $(CFLAGS) $(LDFLAGS) -ldl -o $@

hsakernel.o: omp_veccopy
	cp $< $@
#	objcopy -j hsa_code -j hsa_operand -j hsa_data -j .brig_debug $< $@
