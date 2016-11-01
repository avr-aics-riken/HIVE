#!/bin/sh -x

#PJM --rsc-list "node=2"
#PJM --rsc-list "elapse=0:01:00"

#PJM --stg-transfiles all
#PJM --stgin "./hrender ./"
#PJM --stgin "./bunny.obj ./"
#PJM --stgin "./ObjLoader.so ./"
#PJM --stgin "./glsl/* ./glsl/"
#PJM --stgin "./glsl/bin/* ./glsl/bin/"
#PJM --stgin "./glsl/bin/sparc64/* ./glsl/bin/sparc64/"
#PJM --stgin "./libLSGLES_mpi.so ./"
#PJM --stgin "./libLSGLES_mpi.so.0 ./"
#PJM --stgin "./libhiveCore.so ./"
#PJM --stgin "./normal.frag ./"
#PJM --stgin "./liblua.so ./"
#PJM --stgin "./render_obj.scn ./"
#PJM --stgout "./render_obj.jpg ./"
#PJM -s
#

. /work/system/Env_base
#

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
chmod 755 ./glsl/bin/sparc64/glsl_compiler 

./glsl/bin/sparc64/glsl_compiler

mpiexec -n 2 ./hrender render_obj.scn

