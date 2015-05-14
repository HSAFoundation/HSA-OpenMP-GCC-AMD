GCCHSAIL=$(GCC_HSA)/bin/gcc
LDFLAGS=-L $(GCC_HSA)/lib64 -Wl,-rpath,$(GCC_HSA)/lib64 -L$(HSA_LIBRARY_PATH) -lhsa-runtime64 -lhsakmt
CFLAGS=-lm -fopenmp -Wall -O3
all : matmul
clean :
	rm -rf *.o matmul test.output
# GCC Specifics
matmul.o :  matmul.c
	$(GCCHSAIL) -c $(CFLAGS) $< 
matmul : matmul.o 
	$(GCCHSAIL) matmul.o $(CFLAGS) -o $@  $(LDFLAGS)

