GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 

CFLAGS=-lm -fopenmp -Wall -O3
all : bfs 
clean :
	rm -rf *.o bfs *.hsail *.brig result.txt *hsagen

# GCC Specifics
bfs.o : bfs.cpp
	$(GCCHSAIL) -c  $(CFLAGS) $< 
bfs: bfs.o
	$(GCCHSAIL)  bfs.o $(CFLAGS) -o $@  $(LDFLAGS)

