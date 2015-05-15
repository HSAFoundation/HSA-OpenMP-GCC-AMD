#This should point to top of the directory obtained after downloading from this github repo: https://github.com/HSAFoundation/HSA-Runtime-AMD
HSA_RUNTIME_PATH=$HOME/hsa/HSA-Runtime-AMD
export HSA_LIBRARY_PATH=$HSA_RUNTIME_PATH/lib

# GCC built from HSA branch - https://gcc.gnu.org/svn/gcc/branches/hsa
export GCC_HSA=`pwd | cut -d/ -f-5`/inst
export LD_LIBRARY_PATH=$HSA_LIBRARY_PATH:$GCC_HSA/lib64:$GCC_HSA/lib:$LD_LIBRARY_PATH
