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
    # You may run cmake twice to tell OPENMP flag to SURFACE module correctly.
    # (This looks like a problem of cmake...)
    $ cmake ../ -DHIVE_BUILD_WITH_OPENMP=On
    $ make
    

### MacOSX OpenMP build with GCC

Download gcc from HPC on MacOSX <http://hpc.sourceforge.net>

    $ cd HIVE
    $ mkdir build
    $ ../scripts/cmake_macosx_omp.sh
    $ ../scripts/cmake_macosx_omp.sh # Run twice to ensure telling OpenMP flag to SURFACE module.
    $ make
    
### Setup HIVE

    $ cd build/bin/HIVE
    $ npm install
    
### Run HIVE

    $ cd build/bin/HIVE
    $ node server.js
    

### Setup SceneNodeEditor

    $ cd build/bin/SceneNodeEditor
    $ npm install
    
### Run SceneNodeEditor

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
  * [ ] pyramid element
  * [ ] pentahedron element
  * [x] hexahedron element(converted to triangle)
  * [x] custom attributes
    * float and vec3 type

* CDMlib
  * [x] float precision voxel type(scalar and vector)
  * [x] double precision voxel type(scalar and vector)
    * Convert to float precision for internal representation
  * [x] non-uniform spacing 

* PDMlib
  * [x] particle data
  * [x] custom attributes
    * float, vec2, vec3, vec4 and uint type

