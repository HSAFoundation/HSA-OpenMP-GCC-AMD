GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 

CFLAGS=-lm -fopenmp -Wall $(OPTFLAGS)
#CFLAGS=-lm -fopenmp -Wall -O3

# C compiler
CC = gcc
CC_FLAGS = -g -O3 -Wall -fopenmp -I$(MATRIX_DIR)

# Matrix library path
MATRIX_DIR = ../meschach_lib

track_ellipse.o:
	$(GCCHSAIL) $(CFLAGS) -I$(MATRIX_DIR)  track_ellipse.c -c	
find_ellipse.o:
	$(GCCHSAIL) $(CFLAGS) -I$(MATRIX_DIR)  find_ellipse.c -c	
leukocyte: detect_main.o avilib.o find_ellipse.o track_ellipse.o misc_math.o $(MATRIX_DIR)/meschach.a
	$(CC) $(CFLAGS) $(CC_FLAGS) -lm avilib.o find_ellipse.o track_ellipse.o misc_math.o detect_main.o -o leukocyte -lm $(MATRIX_DIR)/meschach.a -lpthread $(LDFLAGS)

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c

detect_main.o: find_ellipse.h track_ellipse.h avilib.h
find_ellipse.o: avilib.h
track_ellipse.o: find_ellipse.h track_ellipse.h avilib.h

$(MATRIX_DIR)/meschach.a:
	cd $(MATRIX_DIR); ./configure --with-all; make all; make clean

clean:
	rm -f *.o leukocyte

hsakernel.o: leukocyte
	objcopy -O elf32-i386 -j hsa_code -j hsa_operand -j hsa_data -j .brig_debug $< $@

