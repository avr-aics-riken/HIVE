#!/bin/sh

topdir=`pwd`
installdir=`pwd`/third_party/local

# first clean install dir.
rm -rf ${installdir}

#
# TextParser
#
cd third_party/ 
cd TextParser/
if [ -f "Makefile" ]; then
	make distclean
fi

./configure --prefix=${installdir} && make && make install
cd ${topdir}

#
# CDMlib
#
cd third_party/
cd CDMlib/
if [ -f "Makefile" ]; then
	make distclean
fi
CXX=mpicxx CC=mpicc ./configure --prefix=${installdir} --with-parser=${installdir} --with-MPI=yes && make && make install
cd ${topdir}
 
