#!/bin/sh

CXX=mpicxx CC=mpicc cmake28 -H. -Bbuild -DHIVE_BUILD_WITH_MPI=On -DHIVE_BUILD_WITH_CDMLIB=On
