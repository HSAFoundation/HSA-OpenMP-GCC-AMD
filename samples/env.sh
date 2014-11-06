#This should point to top of the directory obtained after downloading from this github repo: https://github.com/HSAFoundation/Okra-Interface-to-HSA-Device
HSA_OKRA_PATH=<EDIT PATH>
#This should point to top of the directory obtained after downloading from this github repo: https://github.com/HSAFoundation/HSA-Runtime-AMD
HSA_RUNTIME_PATH=<EDIT PATH>
#This should point to the libhsakmt directory obtained after downloading from this github repo: https://github.com/HSAFoundation/HSA-Drivers-Linux-AMD/tree/master/kfd-0.8/libhsakmt
HSA_KMT_PATH=<EDIT PATH>
#OKRA has been tested only with 64 bit, so make sure you point to 64-bit binaries of HSA Runtime and KMT libraries
HSA_LIBRARY_PATH=$HSA_OKRA_PATH/okra/dist/bin:$HSA_RUNTIME_PATH/lib/x86_64:$HSA_KMT_PATH/lnx64a:$LD_LIBRARY_PATH

# GCC built from HSA branch - https://gcc.gnu.org/svn/gcc/branches/hsa
export GCC_HSA=<EDIT PATH>
export LD_LIBRARY_PATH=$HSA_LIBRARY_PATH:$GCC_HSA/lib64:$GCC_HSA/lib32:$LD_LIBRARY_PATH
