GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64 
CFLAGS=-lm -fopenmp -Wall -O0 # Pass
#CFLAGS=-lm -fopenmp -Wall -O3 # Crashes TransformScratch

all : omp_veccopy hsakernel.o
clean :
	rm -rf *.o omp_veccopy *.hsail *.brig

launchKernel.o: launchKernel.c launchKernel.h 
	$(CC) -c -g -O0 $< -I$(OKRA_INC) -I/home/gccuser/sekanath/obsedian/okra_obsedian/hsa/include/hsa
# GCC Specifics
omp_veccopy.o :  omp_veccopy.c
	$(GCCHSAIL) -c $(CFLAGS) $< 
omp_veccopy : omp_veccopy.o launchKernel.o 
	g++ -g  launchKernel.o omp_veccopy.o $(CFLAGS) -o $@  $(LDFLAGS) -ldl -L/home/gccuser/sekanath/obsedian/okra_obsedian/dist/bin -lhsail

hsakernel.o: omp_veccopy.o omp_veccopy
	objcopy -O elf32-i386 -j hsa_code -j hsa_operand -j hsa_data -j .brig_debug $< $@
