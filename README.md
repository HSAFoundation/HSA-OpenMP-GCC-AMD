
This repository is used to test OpenMP4.5 in the gcc6.0 development compiler for HSA systems.
Since the development tree is somewhat volitile, we have built some versions of the compiler 
that have some level of functionality.   These are stored in the directory usr/local/hsagccver.
There is a symbolic link /usr/local/hsagcc to the most recent version. 

## Simple Testing

There are two sets of examples.  The smaller set is in the "samples" directory.   The larger set including some Rodinia benchmarks are located in the "examples" directory.  These examples will use the compiler installed at /usr/local/hsagcc

### Samples: 

#### Sample1: vectorCopy
```
* $ cd samples/vectorCopy
* $ make
* $ ./run.sh
   Vector Copy - Passed
```

#### Sample2: matrixMultiply
```
* $ cd samples/matrixMultiply/
* $ make
* $ ./run.sh
    Matrix multiplication - Passed
```
 
See the README in the examples directory for information about testing OpenMP.

## Advanced Testing with GCC libgomp testsuite

The compiler is tested with the gcc libgomp testsuite.  You must build the compiler from source using the hsa branch of the gcc development tree to run the GCC libgomp testsuite. Your system must have all the dependencies such as isl, gmp, etc to build a gcc compiler. These commands will download the source and build the compiler:

```
mkdir -p ~/gcc/src
cd ~/gcc
svn checkout svn://gcc.gnu.org/svn/gcc/hsa src
# Or to get update an existing src directory  "cd src; svn update"
mkdir -p ~/gcc/build
cd build
../src/configure --enable-offload-targets=hsa --with-hsa-runtime=/opt/hsa --disable-bootstrap --enable-languages=c,c++,fortran --prefix=/usr/local/hsagcc  --disable-multilib | tee ../config.out
make -j4 2>&1 | tee make.out
```

After a successful build, you can run the libgomp testsuite.
Use these commands to test different directories in the testsuite. 

```
export LD_LIBRARY_PATH=/opt/hsa/lib
export HSA_DEBUG=1
cd ~/gcc/build
make -k check-target-libgomp RUNTESTFLAGS="--directory=libgomp.c++"
make -k check-target-libgomp RUNTESTFLAGS="--directory=libgomp.fortran"
make -k check-target-libgomp RUNTESTFLAGS="--directory='libgomp.c' c.exp=*" 
```

To install the compiler in /usr/local/hsagcc, you can run this command:

```
cd ~/gcc/build
sudo rm /usr/local/hsagcc
sudo make install-strip
```

