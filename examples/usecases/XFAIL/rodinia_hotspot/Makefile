# C compiler
CC = g++
#CC_FLAGS = -g -fopenmp -O2
CC_FLAGS = -g -fopenmp $(OPTFLAGS)

hotspot: 
	$(CC) $(CC_FLAGS) hotspot_openmp.cpp -o hotspot 

clean:
	rm -f hotspot
