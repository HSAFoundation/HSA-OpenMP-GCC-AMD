GCCHSAIL=$(GCCINSTALLDIR)/bin/g++
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl
CFLAGS=-lm -fopenmp -Wall $(OPTFLAGS)
#CFLAGS=-lm -fopenmp -Wall -O3
#CC = g++
SRC = pathfinder.cpp
EXE = pathfinder
FLAGS = -fopenmp
all: release 
pathfinder.o: pathfinder.cpp
	$(GCCHSAIL) -c  $(CFLAGS) $<
release: pathfinder.o  hsakernel.o
	$(CPP) $(CFLAGS) pathfinder.o -o pathfinder $(LDFLAGS)
#	$(GCCHSAIL) $(SRC) $(CFLAGS) -o $(EXE)

#debug: $(SRC)
#	$(GCCHSAIL) $(SRC) -g -Wall -o $(EXE)
hsakernel.o: pathfinder.o 
	objcopy -O elf32-i386 -j hsa_code -j hsa_operand -j hsa_data -j .brig_debug $< $@

clean:
	rm -f pathfinder *.o


