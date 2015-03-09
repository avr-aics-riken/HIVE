#!/bin/bash

topdir=`pwd`
installdir=`pwd`/third_party/local
c_compiler=mpifccpx
cxx_compiler=mpiFCCpx
cxx_flags="-Kfast"
f90_flags="-Kfast"
f90_compiler=mpifrtpx
if [ -z "${CMAKE_BIN+x}" ]; then
  CMAKE_BIN=/opt/local/bin/cmake
fi

function clean_install_dir {
	rm -rf ${installdir}
}

function build_tp {
	#
	# TextParser
	#
	cd third_party/ 
	cd TextParser/
	if [ -f "Makefile" ]; then
		make distclean
	fi

	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXX_FLAGS=${cxx_flags} ./configure --host=sparc64-unknown-linux-gnu --prefix=${installdir}/TextParser && make && make install
	cd ${topdir}
}

function build_cdmlib {
	#
	# CDMlib
	#
	cd third_party/
	cd CDMlib/
	if [ -f "Makefile" ]; then
		make distclean
	fi
	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} F90=${f90_compiler} F90FLAGS=${f90_flags} ./configure --prefix=${installdir}/CDMlib --with-parser=${installdir}/TextParser --host=sparc64-unknown-linux-gnu --with-MPI=yes && make && make install
	cd ${topdir}
}

function build_polylib {
	#
	# Polylib
	#
	cd third_party/
	cd Polylib/
	if [ -f "Makefile" ]; then
		make distclean
	fi
	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS="-Kfast,ocl,preex,simd=2,uxsimd,array_private,parallel,openmp" ./configure --prefix=${installdir}/Polylib --with-parser=${installdir}/TextParser --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}
}
 
function build_bcmtools {
	#
	# BCMTools
	#
	cd third_party/
	cd BCMTools/
	if [ -f "Makefile" ]; then
		make distclean
	fi
	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} ./configure --prefix=${installdir}/BCMTools --with-parser=${installdir}/TextParser --with-polylib=${installdir}/Polylib --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}
}

function build_hdmlib {
	#
	# HDMlib
	#
	cd third_party/
	cd HDMlib/
	if [ -f "Makefile" ]; then
		make distclean
	fi
	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} ./configure --prefix=${installdir}/HDMlib --with-parser=${installdir}/TextParser --with-bcm=${installdir}/BCMTools --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}
}

function build_pdmlib {
	cd third_party/
	rm -rf fpzip-1.0.1/
	tar -zxvf fpzip-1.0.1.tar.gz
	cp fpzip.Makefile.k fpzip-1.0.1/src
	cd fpzip-1.0.1/src
	# TODO: Provide our own Makefile
	make -f fpzip.Makefile.k
	cd ..
	mkdir -p ${installdir}/include
	mkdir -p ${installdir}/lib
	cp -Rf inc/* ${installdir}/include/
	cp -Rf lib/* ${installdir}/lib/
	cd ${topdir}

	cd third_party/
	rm -rf Zoltan_v3.81/
	rm -rf Zoltan_build/
	tar -zxvf zoltan_distrib_v3.81.tar.gz
	mkdir Zoltan_build
	cd Zoltan_build
	CXX=${cxx_compiler} CC=${c_compiler} CXX_FLAGS=${cxx_flags} ../Zoltan_v3.81/configure --prefix=${installdir} --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}

	#
	# PDMlib
	#
	cd third_party/
	rm -rf PDMlib_build
	mkdir PDMlib_build
	cd PDMlib_build/
	CXX=${cxx_compiler} CC=${c_compiler} ${CMAKE_BIN} -DCMAKE_CXX_COMPILER=${cxx_compiler} -DCMAKE_C_COMPILER=${c_compiler} -DTP_ROOT=${installdir}/TextParser -DFPZIP_ROOT=${installdir} -DZOLTAN_ROOT=${installdir} -DCMAKE_INSTALL_PREFIX=${installdir}/PDMlib ../PDMlib && make && make install
	cd ${topdir}
}

function build_compositor {

	cd third_party/ 
	cd 234Compositor/
	if [ -f "Makefile" ]; then
		make distclean
	fi

	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} ./configure --prefix=${installdir}/234Compositor --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}
}


clean_install_dir
build_tp
build_cdmlib
build_polylib
build_bcmtools
build_hdmlib
build_pdmlib
build_compositor