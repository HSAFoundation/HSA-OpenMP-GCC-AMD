GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64 -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 
CFLAGS=-lm -fopenmp -Wall -O3

all : needle
clean :
	rm -rf *.o needle 

# GCC Specifics
needle.o : needle.cpp
	$(GCCHSAIL) -c  $(CFLAGS)  $<
needle: needle.o
	$(GCCHSAIL)  needle.o $(CFLAGS) -o $@  $(LDFLAGS)
