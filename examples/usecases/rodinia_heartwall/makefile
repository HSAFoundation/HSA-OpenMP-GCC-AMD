GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -L$(OKRA_BIN) -Wl,-rpath,$(OKRA_BIN) 
CFLAGS=-lm -fopenmp -Wall -O2

all : heartwall 

# link objects(binaries) together
heartwall: main.o ./AVI/avilib.o ./AVI/avimod.o
	$(GCCHSAIL) $(CFLAGS) main.o ./AVI/avilib.o ./AVI/avimod.o $(LDFLAGS) -o heartwall -lm

# compile main function file into object (binary)
main.o: main.c define.c kernel.c
	$(GCCHSAIL) main.c -I./AVI  $(CFLAGS) -c 

./AVI/avilib.o ./AVI/avimod.o:
	cd AVI; make;

# delete all object files
clean:
	rm -f *.o AVI/*.o heartwall *.hsail *.brig
