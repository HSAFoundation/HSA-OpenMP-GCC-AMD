all: avimod.o avilib.o

# compile supporting function file into object (binary)
avimod.o: avilib.h avimod.c 
	/usr/bin/gcc -c avimod.c

# compile supporting function file into object (binary)
avilib.o: avilib.h avilib.c
	/usr/bin/gcc -c avilib.c

# delete files
clean:
	rm *.o *.out
