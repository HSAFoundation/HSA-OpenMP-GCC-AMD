GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64 -ldl

CFLAGS=-lm -fopenmp -Wall $(OPTFLAGS)


all : a.out

# Example
# target: dependencies
	# command 1
	# command 2
          # .
          # .
          # .
	# command n

# link objects(binaries) together
a.out:	main.o 
	$(GCCHSAIL) main.o  $(CFLAGS) $(LDFLAGS) -o srad

# compile main function file into object (binary)
main.o: 	main.c \
				define.c \
				graphics.c
	$(GCCHSAIL) main.c $(CFLAGS) -c

# delete all object files
clean:
	rm -rf *.o srad 
