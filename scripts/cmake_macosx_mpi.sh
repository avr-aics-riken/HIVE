#!/bin/sh

# Assume mpicc has been installed with `brew install openmpi`

CXX=mpicxx CC=mpicc cmake ../ -DHIVE_BUILD_WITH_MPI=On
