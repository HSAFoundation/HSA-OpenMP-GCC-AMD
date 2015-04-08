GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl
CFLAGS=-lm -fopenmp -Wall  -O3
#CFLAGS=-lm -fopenmp -Wall -O2
#CFLAGS=-lm -fopenmp -Wall -O3


# C compiler
#CC = gcc
#CC_FLAGS = -g -fopenmp  -O2


backprop: backprop.o facetrain.o imagenet.o backprop_kernel.o 
	$(CC) $(CFLAGS) $(LDFLAGS) backprop.o facetrain.o imagenet.o backprop_kernel.o -o backprop -lm

%.o: %.[ch]
	$(CC) $(CFLAGS) $< -c

facetrain.o: facetrain.c backprop.h
	$(CC) $(CFLAGS) facetrain.c -c
	
backprop.o: backprop.c backprop.h
	$(GCCHSAIL) $(CFLAGS) backprop.c -c

backprop_kernel.o: backprop_kernel.c backprop.h
	$(CC) $(CFLAGS) backprop_kernel.c -c

imagenet.o: imagenet.c backprop.h
	$(CC) $(CFLAGS) imagenet.c -c


clean:
	rm -f *.o *~ backprop *.hsail *.brig
