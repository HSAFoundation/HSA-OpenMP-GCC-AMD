GCCHSA=/opt/rocm/gcc6
HSART=/opt/rocm/hsa/lib
GCCHSABIN=$(GCCHSA)/bin/gcc
LDFLAGS=-L $(GCCHSA)/lib64 
CFLAGS=-lm -fopenmp -Wall -O3
all : matmul
clean :
	rm -rf *.o matmul test.output
matmul.o :  matmul.c
	$(GCCHSABIN) -c $(CFLAGS) $< 
matmul : matmul.o 
	$(GCCHSABIN) matmul.o $(CFLAGS) -o $@  $(LDFLAGS)

run: matmul
	LD_LIBRARY_PATH=$(HSART):$(GCCHSA)/lib64; export LD_LIBRARY_PATH; ./matmul > test.output ; diff --brief test.output golden_output
