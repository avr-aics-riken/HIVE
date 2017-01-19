#HIVE

HIVE is the acronym for "Heterogeneously Integrated Visualization Environment" and is built on SURFACE.

[![Build Status](https://travis-ci.org/avr-aics-riken/HIVE.svg?branch=master)](https://travis-ci.org/avr-aics-riken/HIVE)

## Requirements

* cmake 2.8 or later
* Autotools
* MPI C/C++ compiler(optional for macosx and linux x64 environment)
* Zoltan v.3.81(Required when you build HIVE with PDMlib support)
* fpzip 1.0.1(Required when you build HIVE with PDMlib support)
* cgnslib 3.2.1(Required when you build HIVE with UDMlib support)
* netcdf4 4.2.1.1(Required when you build HIVE with CDMlib support)
* HDF5 1.8.10 patch1(Required when you buil HIVE with CDMlib support)
  * Use brew install hdf5 for MacOSX.
* Fortran compiler(e.g. gfortran. Optional. Required for building CDMlib)

## git clone

    $ git clone https://github.com/avr-aics-riken/HIVE.git
    $ cd HIVE
    $ git submodule update --init

In the later, `$HIVE` is used for the directory where you cloned HIVE git repository.
	
## Setup

Firstly, HIVE needs to build third party libraries.

### K/FX10

Download fpzip, Zoltan and cgnslib and put files into `$HIVE/third_party/`, then

    $ cd $HIVE
    $ ./scripts/build_loader_libs_k_cross.sh

### Linux

#### CentOS 6

Assume cmake2.8 and hdf5 has been installed somewhere.

    $ cd $HIVE
    $ CMAKE_BIN=/path/to/cmake28 ./scripts/build_loader_libs_linux-x64.sh

#### Ubuntu 14.04

    $ sudo apt-get install libhdf5-dev
    $ sudo apt-get install gfortran
    $ cd $HIVE
    $ ./scripts/build_loader_libs_linux-x64.sh

### MacOSX

MacOSX requires gcc-4.8 and openmpi, also requres homebrew supplied HDF5(gcc4.8 failed to compile HDF5 1.8.10 patch1).
(gcc-5 is not supported due to New ABI problem)

#### Setup

Assume homebrew is installed.

    $ brew install gcc48
    $ brew install openmpi
    $ brew install hdf5
    $ export OMPI_CC=gcc-4.8
    $ export OMPI_CXX=g++-4.8
    $ cd $HIVE
    $ ./scripts/build_loader_libs_macosx.sh


## How to build HIVE

This is a manual step to build HIVE. Plese see the later section when you want to use predefined cmake build script.

    $ cd $HIVE
    $ mkdir build
    $ cd build

    # Build with minimum dependency
    $ cmake ../
    $ make

    # Or enable OpenMP build(optional)
    $ cmake ../ -DHIVE_BUILD_WITH_OPENMP=On
    # You may run cmake twice to tell OPENMP flag to SURFACE module correctly.
    # (This looks like a problem of cmake...)
    $ cmake ../ -DHIVE_BUILD_WITH_OPENMP=On
    $ make

### Build options

#### Compilers

Use `CXX` and `CC` flag to specify custom compiler. e.g.:

    CXX=mympicxx CC=mympicc /path/to/cmake ...

#### Options

    -DLUA_USE_READLINE=Off -DLUA_USE_CURSES=Off : disable link with curses and readlines in lua

    HIVE_BUILD_UNIT_TEST OFF CACHE BOOL "Build unit test?"
    HIVE_BUILD_K_CROSS_COMPILE OFF CACHE BOOL "Build for K/FX10 target with corss compiling?"
    HIVE_BUILD_WITH_MPI OFF CACHE BOOL "Build with MPI support?"
    HIVE_BUILD_WITH_OPENMP OFF CACHE BOOL "Build with OpenMP support?"
    HIVE_BUILD_SCREEN_PARALLEL OFF CACHE BOOL "Build with screen parallel support?(Also need to enable HIVE_BUILD_WITH_MPI option)"
    HIVE_BUILD_WITH_CDMLIB OFF CACHE BOOL "Build with CDMlib support?(Also need to enable HIVE_BUILD_WITH_MPI option)"
    (You may need manually add `sz` to NETCDF_LIBS in hrender/CMakeList.txt, depending on your system)
    HIVE_BUILD_WITH_PDMLIB OFF CACHE BOOL "Build with PDMlib support?"
    HIVE_BUILD_WITH_HDMLIB OFF CACHE BOOL "Build with HDMlib support?"
    HIVE_BUILD_WITH_UDMLIB OFF CACHE BOOL "Build with UDMlib support?"
    HIVE_BUILD_WITH_BCMTOOLS OFF CACHE BOOL "Build with BCMTools support?(Also need to enable HIVE_BUILD_WITH_MPI option)"
    HIVE_BUILD_WITH_COMPOSITOR OFF CACHE BOOL "Build with 234Compositor support?(Also need to enable HIVE_BUILD_WITH_MPI option)"
    HIVE_BUILD_WITH_PMLIB OFF CACHE BOOL "Build with PMlib support?(Also need to enable HIVE_BUILD_WITH_MPI option)"

### K/FX10

Optionally edit `./scripts/cmake_k_cross.sh`, then

    $ cd $HIVE 
    $ mkdir build
    $ ./scripts/cmake_k_cross.sh
    $ cd build
    $ make

### Linux

#### Intel Compiler

Optionally edit `./scripts/cmake_linux-x64-all-icc.sh`, then

    $ cd $HIVE 
    $ mkdir build
    $ ./scripts/cmake_linux-x64-all-icc.sh
    $ cd build
    $ make

#### GCC

Optionally edit `./scripts/cmake_linux-x64-all.sh`, then

    $ cd $HIVE 
    $ mkdir build
    $ ./scripts/cmake_linux-x64-all.sh
    $ cd build
    $ make

### MacOSX OpenMP + MPI build

    $ cd $HIVE
    $ mkdir build
    $ ./scripts/cmake_macosx_mpi.sh
    $ cd build
    $ make
    
### Setup node.js for HIVE UI

    $ cd $HIVE
    $ cd build/bin/HIVE_UI
    $ npm install
    
### Run HIVE UI

    $ cd $HIVE
    $ cd build/bin/HIVE_UI
    $ node server.js
    

### Setup SceneNodeEditor

    $ cd $HIVE
    $ cd build/bin/SceneNodeEditor
    $ npm install
    
### Run SceneNodeEditor

    $ cd $HIVE
    $ cd build/bin/SceneNodeEditor
    $ node server.js

## Data loaders and formats

* HDMlib
  * [x] float precision voxel type(scalar and vector)
  * [x] double precision voxel type(scalar and vector)
    * Convert to float precision for internal representation

* UDMlib
  * [x] triangle element
  * [ ] quad element
  * [x] tetra element
  * [x] pyramid element
  * [x] prism element
  * [x] hexahedron element
  * [x] custom attributes
    * float and vec3 type

* CDMlib
  * [x] float precision voxel type(scalar and vector)
  * [x] double precision voxel type(scalar and vector)
    * Convert to float precision for internal representation
  * [x] non-uniform spacing 
  * [x] NetCDF4 format support

* PDMlib
  * [x] particle data
  * [x] custom attributes
    * float, vec2, vec3, vec4 and uint type

