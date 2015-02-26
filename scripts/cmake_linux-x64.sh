#!/bin/sh

# Assume mpicc has been installed

#CXX=mpicxx CC=mpicc cmake28 -H. -Bbuild -DHIVE_BUILD_WITH_MPI=On -DHIVE_BUILD_WITH_CDMLIB=On -DHIVE_BUILD_WITH_HDMLIB=On -DHIVE_BUILD_WITH_PDMLIB=On -DBUILD_SHARED_LIBS=Off
cmake28 -H. -Bbuild -DBUILD_SHARED_LIBS=Off
