#!/bin/sh

# Assume openmpi with C++ binding are installed.
#   $ brew install openmpi --with-cxx-binding

# To enable OpenMP (-fopenmp)
#   $ brew install llvm
#   $ export PATH=/usr/local/opt/llvm/bin:$PATH
#   $ export LD_LIBRARY_PATH="/usr/local/opt/llvm/lib:$LD_LIBRARY_PATH"
#   $ export LIBRARY_PATH="/usr/local/opt/llvm/lib:$LIBRARY_PATH"#

#   For some reason, custom gcc(e.g. gcc installed through homebrew) does not work on Sierra,
#   Thus we forced to use Apple gcc(clang) which does not support OpenMP.

if [ -z "${CMAKE_BIN+x}" ]; then
	CMAKE_BIN=cmake
fi

# Assume mpicc has been installed with `brew install openmpi`
# Inlcude /usr/local/lib (homebrew lib path) to find NetCDF, HDF5, etc.
CXX=mpicxx CC=mpicc ${CMAKE_BIN} -DCMAKE_SHARED_LINKER_FLAGS="-L/usr/local/lib" -H. -Bbuild -DLUA_USE_READLINE=Off -DLUA_USE_CURSES=Off -DBUILD_SHARED_LIBS=On -DHIVE_BUILD_WITH_MPI=On -DHIVE_BUILD_WITH_OPENMP=On -DHIVE_BUILD_WITH_HDMLIB=On -DHIVE_BUILD_WITH_PDMLIB=On -DHIVE_BUILD_WITH_UDMLIB=On -DHIVE_BUILD_WITH_CDMLIB=On -DHIVE_BUILD_WITH_CPMLIB=On -DHIVE_BUILD_WITH_COMPOSITOR=On -DHIVE_BUILD_WITH_PMLIB=On -DCMAKE_BUILD_TYPE=Release
