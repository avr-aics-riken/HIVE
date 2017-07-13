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

	rm -rf TextParser_build
	mkdir -p TextParser_build
	cd TextParser_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/TextParser -DCMAKE_TOOLCHAIN_FILE=../TextParser/cmake/Toolchain_K.cmake -Dwith_MPI=yes -Denable_fapi=yes ../TextParser && make VERVOSE=1 && make install
	if [[ $? != 0 ]]; then exit $?; fi

	cd ${topdir}
}

function build_cdmlib {
	echo "====> Building CDMlib..."

	#
	# CDMlib
	#
	cd third_party/
	#cd CDMlib/


	rm -rf CDMlib_build
	mkdir -p CDMlib_build
	cd CDMlib_build


	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DINSTALL_DIR=${installdir}/CDMlib -DCMAKE_TOOLCHAIN_FILE=../CDMlib/cmake/Toolchain_K.cmake -Dwith_MPI=yes -Dwith_example=yes -Dwith_util=no -Dwith_TP=${installdir}/TextParser -Dwith_CPM=${installdir}/CPMlib -Dwith_NetCDF=/opt/aics/netcdf/k -Denable_BUFFER_SIZE=no ../CDMlib && make VERVOSE=1 && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	echo "====> Built CDMlib."
}

function build_polylib {
	echo "====> Building Polylib..."

	#
	# Polylib
	#
	cd third_party/

	rm -rf Polylib_build
	mkdir -p Polylib_build
	cd Polylib_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DCMAKE_TOOLCHAIN_FILE=../Polylib/cmake/Toolchain_K.cmake -DINSTALL_DIR=${installdir}/Polylib -Dwith_MPI=yes -Dwith_TP=${installdir}/TextParser -Dreal_type=float -Dwith_example=no ../Polylib && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	echo "====> Built Polylib."
}
 
function build_bcmtools {
	echo "====> Building BCMTools..."

	#
	# BCMTools
	#
	cd third_party/
	rm -rf BCMTools_build
	mkdir BCMTools_build
	cd BCMTools_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DCMAKE_TOOLCHAIN_FILE=../BCMTools/cmake/Toolchain_K.cmake -DINSTALL_DIR=${installdir}/BCMTools -Denable_OPENMP=yes -Dwith_MPI=yes -Dwith_example=no -Dwith_TP=${installdir}/TextParser -Dwith_PL=${installdir}/Polylib -Dreal_type=float -Denable_LARGE_BLOCK=no ../BCMTools && make && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	echo "====> Built BCMTools."
}

function build_hdmlib {
	echo "====> Building HDMlib..."

	#
	# HDMlib
	#
	cd third_party/

	rm -rf HDMlib_build
	mkdir -p HDMlib_build
	cd HDMlib_build

	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags} ${CMAKE_BIN} -DCMAKE_TOOLCHAIN_FILE=../HDMlib/cmake/Toolchain_K.cmake -DINSTALL_DIR=${installdir}/HDMlib -Denable_OPENMP=yes -Dwith_MPI=yes -Dwith_example=no -Dwith_TP=${installdir}/TextParser -Dwith_BCM=${installdir}/BCMTools -Dwith_PL=no -Dreal_type=float ../HDMlib && make && make install

	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	echo "====> Built HDMlib."
}

function build_pdmlib {
	echo "====> Building PDMlib..."

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
	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags}  ${CMAKE_BIN} -Dwith_TP=${installdir}/TextParser -Dwith_FPZIP=${installdir} -Dwith_ZOLTAN=${installdir} -DINSTALL_DIR=${installdir}/PDMlib -Dbuild_h5part_converter=no -Dbuild_fv_converter=no -Dbuild_vtk_converter=no ../PDMlib && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	echo "====> Built PDMlib."
}

function build_udmlib {
	echo "====> Building UDMlib..."
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
	cd third_party

	rm -rf UDMlib_build
	mkdir -p UDMlib_build
	cd UDMlib_build

	# Use system installed HDF5 and SZIP
	CXX=${cxx_compiler} CC=${c_compiler} CFLAGS=${c_flags} CXXFLAGS=${cxx_flags}  ${CMAKE_BIN} -DCMAKE_TOOLCHAIN_FILE=../UDMlib/cmake/Toolchain_K.cmake -Dwith_TP=${installdir}/TextParser -Dwith_ZOLTAN=${installdir} -Dwith_CGNS=${installdir} -Dwith_MPI=yes -Dreal_type=float -Dwith_util=no -Dwith_example=no -Dwith_HDF5=/opt/aics/netcdf/k -Dwith_SZIP_LIB=/opt/aics/netcdf/k/lib -Dwith_Z_LIB=/usr/lib64 -DINSTALL_DIR=${installdir}/UDMlib ../UDMlib && make && make install
	if [[ $? != 0 ]]; then exit $?; fi
	cd ${topdir}

	echo "====> Built UDMlib."
}

function build_compositor {

	echo "====> Building 234Compositor..."

	cd third_party/ 
	cd 234Compositor/
	# if [ -f "Makefile" ]; then
	# 	make distclean
	# fi

	#autoreconf -ivf

	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS="${cxx_flags} -Kopenmp" CFLAGS="${c_flags} -Kopenmp" ./configure --prefix=${installdir}/234Compositor --host=sparc64-unknown-linux-gnu && make && make install
	if [[ $? != 0 ]]; then exit $?; fi

	cd ${topdir}

	echo "====> Built 234Compositor."
}

function build_nanomsg {

	echo "====> Building nanomsg..."

	cd third_party/ 
	cd nanomsg/
	if [ -f "Makefile" ]; then
		make distclean
	fi

	autoreconf -ivf
	CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS=${cxx_flags} CFLAGS=${c_flags} ./configure --host=sparc64-unknown-linux-gnu --prefix=${installdir}/nanomsg && make && make install
	if [[ $? != 0 ]]; then exit $?; fi

	cd ${topdir}

	echo "====> Built nanomsg."
}

function build_pmlib {

	echo "====> Building PMlib..."

        cd third_party/PMlib
        autoreconf -ivf

        rm -rf BUILD_DIR
        mkdir -p BUILD_DIR
        cd BUILD_DIR
        CXX=${cxx_compiler} CC=${c_compiler} CXXFLAGS="${cxx_flags} -Kopenmp -Ntl_notrt -Nfjcex -DK_COMPUTER -W" CFLAGS="${c_flags} -Kopenmp -Ntl_notrt  -Nfjcex -DL_COMPUTER -W" ../configure --prefix=${installdir}/PMlib --with-comp="FJ" LDFLAGS="-L/opt/FJSVXosDevkit/sparc64fx/V01L02E07/target/usr/lib64" --host=sparc64-unknown-linux-gnu && make && make install
        if [[ $? != 0 ]]; then exit $?; fi
        cd ${topdir}

	echo "====> Built PMlib."
}

function build_cpmlib {
	echo "====> Building CPMlib...."

        cd third_party/

	rm -rf CPMlib_build
	mkdir CPMlib_build
	cd CPMlib_build

	${CMAKE_BIN} -DINSTALL_DIR=${installdir}/CPMlib -DCMAKE_TOOLCHAIN_FILE=../CPMlib/cmake/Toolchain_K.cmake -Dwith_MPI=yes -Dreal_type=float -Denable_LMR=no -Dwith_example=no -Dwith_TP=${installdir}/TextParser ../CPMlib && make && make install
        if [[ $? != 0 ]]; then exit $?; fi

        cd ${topdir}

	echo "====> Built CPMlib."
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
build_compositor
build_pmlib

# nanomsg is not supported on K platform
#build_nanomsg
