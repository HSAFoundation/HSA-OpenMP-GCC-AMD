GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 -lhsakmt
#CFLAGS=-lm -fopenmp -Wall 
#CFLAGS=-lm -fopenmp -Wall $(OPTFLAGS)
CFLAGS=-lm -fopenmp -Wall -O3

all : srad 

srad.o: srad.cpp
	$(GCCHSAIL) $(CFLAGS) srad.cpp -c
srad: srad.o 
	$(GCCHSAIL) $(CFLAGS) srad.o -o srad $(LDFLAGS) -lm

clean:
	rm -f srad launchKernel.o *.o test.output
