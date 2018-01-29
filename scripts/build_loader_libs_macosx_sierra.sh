#!/bin/sh

# OpenMP is disabled.

topdir=`pwd`
installdir=`pwd`/third_party/local

# Path to homebew intalled NetCDF dir
netcdf_dir=/usr/local/

if [ -z "${CMAKE_BIN+x}" ]; then
  CMAKE_BIN=cmake
fi

if [ -z "${CC+x}" ]; then
  c_compiler=mpicc
  CC=mpicc
else
  c_compiler=${CC}
fi

if [ -z "${FC+x}" ]; then
  fc_compiler=gfortran-5
  FC=gfortran-5
else
  fc_compiler=${FC}
fi

if [ -z "${CXX+x}" ]; then
  cxx_compiler=mpicxx
  CXX=mpicxx
else
  cxx_compiler=${CXX}
fi

echo "C compiler  : " ${CC}
echo "CXX compiler: " ${CXX}
echo "FC compiler: " ${FC}

set -e

function clean_install_dir {
	rm -rf ${installdir}
}

# Must use TP version 1.7.4 which includes tp-config(tp-config is requred for building UDMlib)
function build_tp {
	#
	# TextParser
	#
	cd third_party/ 

	rm -rf TextParser_build
	mkdir -p TextParser_build
	cd TextParser_build
	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/TextParser -Dwith_MPI=yes -Denable_test=no -Denable_fapi=no ../TextParser && make && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

}

# Assume NetCDF4 and HDF5 are installed with homebrew 
function build_cdmlib {
	#
	# CDMlib
	#
	cd third_party/

	rm -rf CDMlib_build
	mkdir -p CDMlib_build
	cd CDMlib_build

	CXX=${cxx_compiler} CC=${c_compiler} FC=${fc_compiler} F90=${fc_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/CDMlib -Dwith_MPI=yes -Dwith_TP=${installdir}/TextParser -Dwith_NetCDF=${netcdf_dir} -Dwith_util=no -Dwith_example=no ../CDMlib && make && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}

function build_polylib {
	#
	# Polylib
	#
	cd third_party/

	rm -rf Polylib_build
	mkdir -p Polylib_build
	cd Polylib_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/Polylib -Dwith_MPI=yes -Dwith_TP=${installdir}/TextParser -Dreal_type=float -Dwith_example=no ../Polylib && make && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}
 
function build_bcmtools {
	#
	# BCMTools
	#
	cd third_party/

	rm -rf BCMTools_build
	mkdir -p BCMTools_build
	cd BCMTools_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/BCMTools -Denable_OPENMP=no -Dwith_MPI=yes -Dwith_TP=${installdir}/TextParser -Dwith_PL=${installdir}/Polylib ../BCMTools && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}

function build_hdmlib {
	#
	# HDMlib
	#
	cd third_party/

	rm -rf HDMlib_build
	mkdir -p HDMlib_build
	cd HDMlib_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/HDMlib -Denable_OPENMP=no -Dwith_MPI=yes -Dwith_TP=${installdir}/TextParser -Dwith_PL=${installdir}/Polylib -Dwith_BCM=${installdir}/BCMTools -Dreal_type=float ../HDMlib && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}

function build_pdmlib {
	cd third_party/
	rm -rf fpzip-1.0.1/
	tar -zxvf fpzip-1.0.1.tar.gz
	cd fpzip-1.0.1/src
	# TODO: Provide our own Makefile
	CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} make
	if [[ $? != 0 ]]; then exit $?; fi
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

	# Apply a patch for perl script(e.g. Newer Perl 5.22 on Ubuntu 16.04 fails to exec without this patch)
	cd Zoltan_v3.81
	patch -p0 < ../zoltan_installscript_patch.diff
	cd ..

	mkdir Zoltan_build
	cd Zoltan_build
	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ../Zoltan_v3.81/configure --prefix=${installdir} && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	#
	# PDMlib
	#
	cd third_party/
	rm -rf PDMlib_build
	mkdir PDMlib_build
	cd PDMlib_build/
	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags}  ${CMAKE_BIN} -Dwith_TP=${installdir}/TextParser -Dwith_FPZIP=${installdir} -Dwith_ZOLTAN=${installdir} -DINSTALL_DIR=${installdir}/PDMlib -Dbuild_h5part_converter=no -Dbuild_fv_converter=no -Dbuild_vtk_converter=no ../PDMlib && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}

function build_udmlib {
	cd third_party/
	rm -rf cgnslib_3.2.1/
	rm -rf cgnslib_build/
	tar -zxvf cgnslib_3.2.1.tar.gz
	mkdir cgnslib_build
	cd cgnslib_build
	# Create .a only(disable .dylib build)
	CXX=${cxx_compiler} CC=${c_compiler} ${CMAKE_BIN} -DCMAKE_INSTALL_PREFIX=${installdir} -DCGNS_ENABLE_64BIT=On -DCGNS_BUILD_SHARED=Off ../cgnslib_3.2.1 && make VERBOSE=1 && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	cd third_party/
	rm -rf Zoltan_v3.81/
	rm -rf Zoltan_build/
	tar -zxvf zoltan_distrib_v3.81.tar.gz
	mkdir Zoltan_build
	cd Zoltan_build
	CXX=${cxx_compiler} CC=${c_compiler} ../Zoltan_v3.81/configure --prefix=${installdir} && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	#
	# UDMlib
	#
	cd third_party/UDMlib
	#autoreconf -ivf

	rm -rf BUILD_DIR
	mkdir -p BUILD_DIR
	cd BUILD_DIR

	# Work around: Use cxx compiler even for CC to compile example programs.
	#CXX=${cxx_compiler} CC=${cxx_compiler} ../configure --prefix=${installdir}/UDMlib --with-tp=${installdir}/TextParser --with-zoltan=${installdir} --with-cgns=${installdir} && make && make install
	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags}  ${CMAKE_BIN} -Dwith_TP=${installdir}/TextParser -Dwith_ZOLTAN=${installdir} -Dwith_CGNS=${installdir} -Dwith_MPI=yes -Dreal_type=float -Dwith_util=no -Dwith_example=no -Dwith_HDF5=${installdir} -DINSTALL_DIR=${installdir}/UDMlib .. && make && make install
	
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}

function build_compositor {

	cd third_party/ 
	cd 234Compositor/
	#if [ -f "Makefile" ]; then
	#	make distclean
	#fi

	CXX=${cxx_compiler} CC=${c_compiler} ./configure --prefix=${installdir}/234Compositor && make && make install
	cd ${topdir}
}

function build_pmlib {

        cd ${topdir}/third_party/PMlib

        autoreconf -ivf
        cd BUILD_DIR
        CXX=${cxx_compiler} CC=${c_compiler} ../configure --prefix=${installdir}/PMlib && make && make install
        if [[ $? != 0 ]]; then exit $?; fi
        cd ${topdir}
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

function build_cpmlib {

	cd third_party

	rm -rf CPMlib_build
	mkdir -p CPMlib_build
	cd CPMlib_build

	CXX=${cxx_compiler} CC=${c_compiler} FC=${fc_compiler} F90=${fc_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/CPMlib -Dwith_MPI=yes -Dwith_example=no -Dwith_TP=${installdir}/TextParser -Dreal_type=float -Denable_LMR=no ../CPMlib && make && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}
}


clean_install_dir
build_tp
build_cpmlib
build_cdmlib
build_polylib
build_bcmtools
build_hdmlib
build_pdmlib
build_udmlib
build_pmlib
build_compositor
build_nanomsg
