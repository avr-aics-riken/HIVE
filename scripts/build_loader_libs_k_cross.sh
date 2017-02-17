#!/bin/bash

topdir=`pwd`
installdir=`pwd`/third_party/local
c_compiler=mpifccpx
cxx_compiler=mpiFCCpx
c_flags="-Xg -Kfast -KPIC"
cxx_flags="-Xg -Kfast -KPIC"
f90_flags="-Xg -Kfast -KPIC"
f90_compiler=mpifrtpx
if [ -z "${CMAKE_BIN+x}" ]; then
  CMAKE_BIN=/opt/local/bin/cmake
fi

set -e

function clean_install_dir {
	rm -rf ${installdir}
}

function build_tp {
	#
	# TextParser
	#
	cd third_party/ 
	cd TextParser/

	# It looks like setting CXX/CC require regeneration of configure.
	autoreconf -ivf

	rm -rf build
	mkdir -p build
	cd build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS="${c_flags}" CXXFLAGS=${cxx_flags} ../configure --host=sparc64-unknown-linux-gnu --prefix=${installdir}/TextParser && make && make install
	cd ${topdir}
}

function build_cdmlib {
	#
	# CDMlib
	#
	cd third_party/
	cd CDMlib/

	# It looks like setting CXX/CC require regeneration of configure.
	autoreconf -ivf

	rm -rf build
	mkdir -p build
	cd build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS="${c_flags}" CXXFLAGS="${cxx_flags} -I/opt/aics/netcdf/k/include" LDFLAGS="-L/opt/aics/netcdf/k/lib-static" LIBS="-lnetcdf -lhdf5_hl -lhdf5 -lsz -lz" F90=${f90_compiler} F90FLAGS=${f90_flags} ../configure --prefix=${installdir}/CDMlib --with-parser=${installdir}/TextParser --host=sparc64-unknown-linux-gnu --with-MPI=yes --with-nc && make && make install
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

	# It looks like setting CXX/CC require regeneration of configure.
	autoreconf -ivf

	rm -rf build
	mkdir -p build
	cd build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS="${c_flags}" CXXFLAGS="-Kfast,ocl,preex,simd=2,uxsimd,array_private,parallel,openmp" ../configure --prefix=${installdir}/Polylib --with-parser=${installdir}/TextParser --host=sparc64-unknown-linux-gnu && make && make install
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
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} CFLAGS="${c_flags}" ./configure --prefix=${installdir}/BCMTools --with-parser=${installdir}/TextParser --with-polylib=${installdir}/Polylib --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}
}

function build_hdmlib {
	#
	# HDMlib
	#
	cd third_party/
	cd HDMlib/

	autoreconf -ivf

	rm -rf build
	mkdir -p build
	cd build

	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} CFLAGS=${c_flags} ../configure --prefix=${installdir}/HDMlib --with-parser=${installdir}/TextParser --with-bcm=${installdir}/BCMTools --host=sparc64-unknown-linux-gnu && make && make install
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
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} CFLAGS=${c_flags} ../Zoltan_v3.81/configure --prefix=${installdir} --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}

	#
	# PDMlib
	#
	cd third_party/
	rm -rf PDMlib_build
	mkdir PDMlib_build
	cd PDMlib_build/
	CXX=${cxx_compiler} CC=${c_compiler} ${CMAKE_BIN} -DCMAKE_C_FLAGS=${c_flags} -DCMAKE_CXX_FLAGS=${cxx_flags} -DCMAKE_CXX_COMPILER=${cxx_compiler} -DCMAKE_C_COMPILER=${c_compiler} -DTP_ROOT=${installdir}/TextParser -DFPZIP_ROOT=${installdir} -DZOLTAN_ROOT=${installdir} -DCMAKE_INSTALL_PREFIX=${installdir}/PDMlib -Dbuild_h5part_converter=no -Dbuild_fv_converter=no -Dbuild_vtk_converter=no ../PDMlib && make && make install
	cd ${topdir}
}

function build_udmlib {
	cd third_party/
	rm -rf Zoltan_v3.81/
	rm -rf Zoltan_build/
	tar -zxvf zoltan_distrib_v3.81.tar.gz
	mkdir Zoltan_build
	cd Zoltan_build
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} CFLAGS=${c_flags} ../Zoltan_v3.81/configure --prefix=${installdir} --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}

	cd third_party/
	rm -rf cgnslib_3.2.1/
	rm -rf cgnslib_build/
	tar -zxvf cgnslib_3.2.1.tar.gz
	mkdir cgnslib_build
	cd cgnslib_build
	CXX=${cxx_compiler} CC=${c_compiler} ${CMAKE_BIN} -DCMAKE_C_FLAGS=${c_flags} -DCMAKE_CXX_FLAGS=${cxx_flags} -DCMAKE_INSTALL_PREFIX=${installdir} -DCGNS_ENABLE_64BIT=On -DCGNS_BUILD_SHARED=Off ../cgnslib_3.2.1 && make && make install
	cd ${topdir}

	#
	# UDMlib
	#
	cd third_party/UDMlib
	autoreconf -ivf

	rm -rf build
	mkdir -p build
	cd build

	# Work around: Use cxx compiler even for CC to compile example programs.
	CXX=${cxx_compiler} CC=${cxx_compiler} CXX_FLAGS=${cxx_flags} CFLAGS=${c_flags} ../configure --prefix=${installdir}/UDMlib --with-comp=FJ --host=sparc64-unknown-linux-gnu --with-tp=${installdir}/TextParser --with-zoltan=${installdir} --with-cgns=${installdir} && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}

function build_compositor {

	cd third_party/ 
	cd 234Compositor/
	if [ -f "Makefile" ]; then
		make distclean
	fi

	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS="${cxx_flags} -Kopenmp" CFLAGS="${c_flags} -Kopenmp" ./configure --prefix=${installdir}/234Compositor --host=sparc64-unknown-linux-gnu && make && make install
	cd ${topdir}
}

function build_nanomsg {

	cd third_party/ 
	cd nanomsg/
	if [ -f "Makefile" ]; then
		make distclean
	fi

	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} CFLAGS=${c_flags} ./configure --prefix=${installdir}/nanomsg && make && make install
	cd ${topdir}
}

function build_pmlib {

        cd third_party/PMlib
        autoreconf -ivf

        rm -rf BUILD_DIR
        mkdir -p BUILD_DIR
        cd BUILD_DIR
        CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS="${cxx_flags} -Kopenmp -Ntl_notrt -Nfjcex -DK_COMPUTER -W" CFLAGS="${c_flags} -Kopenmp -Ntl_notrt  -Nfjcex -DL_COMPUTER -W" ../configure --prefix=${installdir}/PMlib --with-comp="FJ" LDFLAGS="-L/opt/FJSVXosDevkit/sparc64fx/V01L02E07/target/usr/lib64" --host=sparc64-unknown-linux-gnu && make && make install
        if [[ $? != 0 ]]; then exit $?; fi
        cd ${topdir}
}

clean_install_dir
build_tp
build_cdmlib
build_polylib
build_bcmtools
build_hdmlib
build_pdmlib
build_udmlib
build_compositor
build_pmlib
# build_nanomsg
