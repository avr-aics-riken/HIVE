#!/bin/sh

topdir=`pwd`
installdir=`pwd`/third_party/local
#c_compiler=mpicc
#cxx_compiler=mpicxx
cmake_bin=cmake

set -e

function clean_install_dir {
	rm -rf ${installdir}
}

function build_nanomsg {

	cd third_party/ 
	cd nanomsg/
	#if [ -f "Makefile" ]; then
	#	make distclean
	#fi

	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} ./configure --prefix=${installdir}/nanomsg && make && make install
	cd ${topdir}
}

clean_install_dir
build_nanomsg
