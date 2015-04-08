#GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
GCCHSAIL=gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64 
CFLAGS=-lm -fopenmp -Wall -O0 # Pass
#CFLAGS=-lm -fopenmp -Wall -O3 # Crashes TransformScratch

all : omp_veccopy hsakernel.o
clean :
	rm -rf *.o omp_veccopy *.hsail *.brig

launchKernel.o: launchKernel.c launchKernel.h 
	$(CC) -c -g -O0 $< -I$(OKRA_INC) 
# GCC Specifics
omp_veccopy : launchKernel.o omp_veccopy.c omp_veccopy2.c
	$(GCCHSAIL) -g  launchKernel.o omp_veccopy.c omp_veccopy2.c -Ofast -flto $(CFLAGS) -o $@  $(LDFLAGS) -ldl

hsakernel.o: omp_veccopy.o omp_veccopy
	objcopy -O elf32-i386 -j .brig_strtab -j .brig_directives -j .brig_code -j .brig_operands -j .brig_debug $< $@
	$(HSAILASM_095) -disassemble hsakernel.o
