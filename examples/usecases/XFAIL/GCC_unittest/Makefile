#Placeholder for negative testcases
GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl
CFLAGS=-lm -fopenmp -Wall $(OPTFLAGS)
#CFLAGS=-lm -fopenmp -Wall -O3
#CFLAGS=-lm -fopenmp -Wall -O3

all : 
	echo "Compile all Negative testcases"	

clean :
	rm -rf *.o *.hsail *.brig

