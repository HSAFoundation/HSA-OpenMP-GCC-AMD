GCCHSAIL=$(GCCINSTALLDIR)/bin/gcc
LDFLAGS_HW=-L $(GCCINSTALLDIR)/lib64 -Wl,-rpath,$(GCCINSTALLDIR)/lib64  -ldl -L$(HSA_RUNTIME_PATH) -lhsa-runtime64 -lhsakmt
CFLAGS=-lm -fopenmp -Wall -O1
all : lud_omp

# Makefile for LU Decomposition
#
#

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := YES

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# ------------  name of the executable  ----------------------------------------
EXECUTABLE      := lud_omp

# ------------  list of all source files  --------------------------------------
SOURCES         := lud.c lud_omp.c ../common/common.c 

# ------------  compiler  ------------------------------------------------------
#CC              := gcc
CXX             := g++

# ------------  compiler flags  ------------------------------------------------
DEBUG_CFLAGS    := -Wall
RELEASE_CFLAGS  := -Wall 
COMMON_CFLAGS   := -fopenmp

# ------------  compiler defs  ------------------------------------------------
DEBUG_CDEFS    := 
RELEASE_CDEFS  := 
COMMON_CDEFS   := 

# ------------  linker flags  --------------------------------------------------
DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  := 
COMMON_LDFLAGS   := -fopenmp 

ifeq (YES, ${DEBUG})
  CFLAGS       := ${DEBUG_CFLAGS} ${DEBUG_CDEFS}
  CXXFLAGS     := ${DEBUG_CXXFLAGS} ${DEBUG_CDEFS}
  LDFLAGS      := ${DEBUG_LDFLAGS}
else
  CFLAGS       := ${RELEASE_CFLAGS} ${RELEASE_CDEFS}
  CXXFLAGS     := ${RELEASE_CXXFLAGS} ${RELEASE_CDEFS}
  LDFLAGS      := ${RELEASE_LDFLAGS}
endif

CFLAGS         := ${CFLAGS} ${COMMON_CFLAGS}
CXXFLAGS       := ${CXXFLAGS} ${COMMON_CFLAGS}
LDFLAGS        := ${LDFLAGS} ${COMMON_LDFLAGS}

ifeq (YES, ${PROFILE})
  CFLAGS       := ${CFLAGS}   -pg
  CXXFLAGS     := ${CXXFLAGS} -pg 
  LDFLAGS      := ${LDFLAGS}  -pg
endif

# ------------  additional system include directories  -------------------------
GLOBAL_INC_DIR  =

# ------------  private include directories  -----------------------------------
LOCAL_INC_DIR   = ../common

# ------------  system libraries  (e.g. -lm )  ---------------------------------
SYS_LIBS        = -lm

# ------------  additional system library directories  -------------------------
GLOBAL_LIB_DIR  =

# ------------  additional system libraries  -----------------------------------
GLOBAL_LIBS     =

# ------------  private library directories  -----------------------------------
LOCAL_LIB_DIR   = 

# ------------  private libraries  (e.g. libxyz.a )  ---------------------------
LOCAL_LIBS      =


C_SOURCES       = $(filter     %.c, $(SOURCES))
CPP_SOURCES     = $(filter-out %.c, $(SOURCES))
ALL_INC_DIR     = $(addprefix -I, $(LOCAL_INC_DIR) $(GLOBAL_INC_DIR))
ALL_LIB_DIR     = $(addprefix -L, $(LOCAL_LIB_DIR) $(GLOBAL_LIB_DIR))
GLOBAL_LIBSS    = $(addprefix $(GLOBAL_LIB_DIR)/, $(GLOBAL_LIBS))
LOCAL_LIBSS     = $(addprefix $(LOCAL_LIB_DIR)/, $(LOCAL_LIBS))
ALL_CFLAGS      = $(CFLAGS) $(ALL_INC_DIR)
ALL_LFLAGS      = $(LDFLAGS) $(ALL_LIB_DIR)
BASENAMES       = $(basename $(SOURCES))

# ------------  generate the names of the object files  ------------------------
OBJECTS         = $(addsuffix .o,$(BASENAMES))
 
# ------------  make the executable (the default goal)  ------------------------
$(EXECUTABLE):	$(OBJECTS)
ifeq ($(strip $(CPP_SOURCES)),)
	$(CC)  $(ALL_LFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS) $(CFLAGS) $(LDFLAGS_HW)
else
	$(CC) $(ALL_LFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS) $(CFLAGS) $(LDFLAGS_HW)
endif

# ------------  make the objects  ----------------------------------------------
%.o:		%.c
				$(GCCHSAIL)  -c $(ALL_CFLAGS) $(CFLAGS) $< -o $@

%.o:		%.cc
				$(CXX) -c $(ALL_CFLAGS) $< -o $@

%.o:		%.cpp
				$(CXX) -c $(ALL_CFLAGS) $< -o $@

%.o:		%.C
				$(CXX) -c $(ALL_CFLAGS) $< -o $@

clean:
	-rm --force $(EXECUTABLE) $(OBJECTS) *~ *.o *.hsail *.brig test.output temp

.PHONY: clean

# ==============================================================================
# vim: set tabstop=2: set shiftwidth=2:
