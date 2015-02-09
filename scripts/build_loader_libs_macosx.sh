#!/bin/sh

topdir=`pwd`
installdir=`pwd`/third_party/local

cd third_party/ 
cd TextParser
if [ -f "Makefile" ]; then
	make distclean
fi
../configure --prefix=${installdir} && make && make install
cd ${topdir}

cd third_party/
cd CDMlib
if [ -f "Makefile" ]; then
	make distclean
fi
./configure --prefix=${installdir} --with-parser=${installdir} --with-MPI=no --without-ompi && make && make install
cd ${topdir}
 
