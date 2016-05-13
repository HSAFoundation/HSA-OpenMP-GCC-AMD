GCCHSAIL=$(GCCINSTALLDIR)/bin/g++
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64   -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 
CFLAGS=-lm -fopenmp -Wall -O3
all : matmul
clean :
	rm -rf *.o matmul *.hsail *.brig
# GCC Specifics
matmul.o :  matmul.c
	$(GCCHSAIL) -fdump-tree-ompexp-details -c $(CFLAGS) $< 
matmul : matmul.o 
	$(GCCHSAIL) matmul.o $(CFLAGS) -o $@  $(LDFLAGS)
#/opt/amd/bin/hsailasm -disassemble matmul.o
