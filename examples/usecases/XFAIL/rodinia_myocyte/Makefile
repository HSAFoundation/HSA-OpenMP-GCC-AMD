GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 -lhsakmt
CFLAGS=-lm -fopenmp -Wall -O0
#CFLAGS=-lm -fopenmp -Wall -O0
#CFLAGS=-lm -fopenmp -Wall -O3
# Example
# target: dependencies
	# command 1
	# command 2
          # .
          # .
          # .
	# command n

# link objects(binaries) together
myocyte.out:	main.o
	gcc	main.o \
			-lm -fopenmp \
	-o myocyte.out

# compile main function file into object (binary)
main.o: 	main.c \
				define.c \
				ecc.c \
				cam.c \
				fin.c \
				master.c \
				embedded_fehlberg_7_8.c \
				solver.c \
				file.c \
				timer.c
	$(GCCHSAIL) $(CFLAGS) $(LDFLAGS)	main.c \
			-c -O3 -fopenmp

# delete all object files
clean:
	rm -rf *.o myocyte.out output.txt *.hsail *.brig
