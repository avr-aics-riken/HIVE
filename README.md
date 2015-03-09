#HIVE

HIVE is the acronym for "Heterogeneously Integrated Visualization Environment" and is built on SURFACE.

## Requirements

* cmake 2.8 or later
* HDF5
* MPI compiler(optional for macosx and linux x64 environment)

## git clone

    $ git clone https://github.com/avr-aics-riken/HIVE.git
    $ git submodule udate --init
	
## Setup

Build third party libraries.

### Linux

#### CentOS 6

Assume cmake2.8 and hdf5 has been installed somewhere.

    $ CMAKE_BIN=/path/to/cmake28 ./scripts/build_loader_libs_linux-x64.sh

#### Ubuntu 14.04

    $ sudo apt-get install libhdf5-dev
    $ ./scripts/build_loader_libs_linux-x64.sh

### MacOSX

    $ ./scripts/build_loader_libs_macosx.sh


## How to build HIVE

    $ cd HIVE
    $ mkdir build
    $ cd build
    $ cmake ../
    # Enable OpenMP build(optional)
    $ cmake ../ -DHIVE_BUILD_WITH_OPENMP=On
    $ make

### MacOSX OpenMP build with GCC

Download gcc from HPC on MacOSX <http://hpc.sourceforge.net>

    $ cd HIVE
    $ mkdir build
    $ ../scripts/cmake_macosx_omp.sh
    $ make
    
