GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 -lhsakmt

CFLAGS=-lm -fopenmp -Wall $(OPTFLAGS)
#CFLAGS=-lm -fopenmp -Wall -O0

PREFIX=${PARSECDIR}/pkgs/kernels/streamcluster/inst/${PARSECPLAT}

TARGET_C = sc_cpu

TARGET_O = sc_omp

ifdef version
  ifeq "$(version)" "parallel"
    CXXFLAGS :=	$(CXXFLAGS) -DENABLE_THREADS -pthread
  endif
endif

all: cpu omp

cpu:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) streamcluster_original.cpp -o $(TARGET_C) $(LIBS) -DENABLE_THREADS -pthread

omp:
	$(GCCHSAIL) -O3 -fopenmp -o $(TARGET_O) streamcluster_omp.cpp


clean:
	rm -f *.o *~ *.txt sc_cpu sc_omp
	
