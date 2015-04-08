GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -L$(OKRA_BIN) -Wl,-rpath,$(OKRA_BIN) -ldl
CC=$(GCCHSAIL)
#CFLAGS=-lm -fopenmp -Wall -O2
CFLAGS=-lm -fopenmp -Wall -O2 -g

all : kmeans 

kmeans: cluster.o kmeans.o kmeans_clustering.o 
	$(CC) $(CFLAGS) cluster.o kmeans.o kmeans_clustering.o  -o kmeans $(LDFLAGS)

%.o: %.[ch]
	$(CC) $(CFLAGS) $< -c

cluster.o: cluster.c 
	$(CC) $(CFLAGS) cluster.c -c
	
kmeans.o: kmeans.c 
	$(CC) $(CFLAGS) kmeans.c -c

kmeans_clustering.o: kmeans_clustering.c kmeans.h
	$(GCCHSAIL) $(CFLAGS) kmeans_clustering.c -c 

clean:
	rm -f *.o *~ kmeans *.brig *.hsail test.output
