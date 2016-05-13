
This repository is used to test the expermental OpenMP4.5 in the gcc6 development compiler for HSA systems. This compiler only works for carrizo and kaveri APUs.   A future version will work with fiji discreet GPU cards. 

## Install ROCM gcc6 compiler

Since this is an experimental gcc compiler, it will not install into system directories, paths,  or libraries.  Your current gcc installation will not be affected.  All installation files will go into /opt/rocm/gcc6.  No links or PATH modification is done by this debian package.  Before installation of rocmgcc6, you must install ROCM.  See

http://gpuopen.com/getting-started-with-boltzmann-components-platforms-installation/

After you install ROCM, install rocmgcc6 as follows. 

```
cd packages/ubuntu
dpkg -i *.deb
```
When this compiler supports discreet GPU cards, it will be available from the ROCM package manager for automatic installation with apt.  

## Simple Testing

There are two sets of examples.  The smaller set is in the "samples" directory.   The larger set including some Rodinia benchmarks are located in the "examples" directory.  These examples Makefiles or run scripts will use the compiler at /opt/rocm/gcc6. This is where the debian package installs the compiler.  

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
# Or to update the existing src directory use this command:  "cd src; svn update"
mkdir -p ~/gcc/build
cd ~/gcc/build
../src/configure --enable-offload-targets=hsa --with-hsa-runtime=/opt/rocm/hsa --with-hsa-kmt-lib=/opt/rocm/libhsakmt --disable-bootstrap --enable-languages=c,c++,fortran --prefix=/opt/rocm/gcc6  --without-isl --disable-multilib --with-system-zlib .. 
make -j4 
```
After a successful build, you can run the libgomp testsuite.
Use these commands to test different directories in the testsuite. 
```
export LD_LIBRARY_PATH=/opt/rocm/hsa/lib
export HSA_DEBUG=1
cd ~/gcc/build
make -k check-target-libgomp RUNTESTFLAGS="--directory=libgomp.c++"
make -k check-target-libgomp RUNTESTFLAGS="--directory=libgomp.fortran"
make -k check-target-libgomp RUNTESTFLAGS="--directory='libgomp.c' c.exp=*" 
```

To install the compiler, you can run this command:
```
cd ~/gcc/build
sudo make install-strip
```
