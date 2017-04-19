cmake_minimum_required(VERSION 2.8)
enable_testing()

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)

# ------------------------------------------------------------

message(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})

# --------------------------------------
# 
#  SAME Build Option with SURFACE
#
# Build options
#
# K/FX10 native environment
IF (CMAKE_SYSTEM_PROCESSOR MATCHES "s64fx")

  # Use native Fujitsu compiler
  IF (HIVE_BUILD_WITH_MPI)
    SET(CMAKE_C_COMPILER mpifcc)
    SET(CMAKE_CXX_COMPILER mpiFCC)
  ELSE()
    SET(CMAKE_C_COMPILER fcc)
    SET(CMAKE_CXX_COMPILER FCC)
  ENDIF()

  # gcc compatible flag(-Xg)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xg")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xg")

  # PIC flag
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

  # OpenMP flag(-Kopenmp)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kopenmp")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kopenmp")

  IF (CMAKE_BUILD_TYPE MATCHES "Release")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast")
  ENDIF()

  IF (HIVE_BUILD_WITH_PMLIB)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Nfjcex -Ntl_notrt -DK_COMPUTER")
    SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Nfjcex -Ntl_notrt -DK_COMPUTER")
  ENDIF()

  # Prevent CMake from adding GNU-specific linker flags (-rdynamic)
  SET(CMAKE_C_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C cross-compiler" FORCE)
  SET(CMAKE_CXX_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C++ cross-compiler" FORCE)

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
ENDIF()

# K cross compiling
IF (HIVE_BUILD_K_CROSS_COMPILE)

  # Use Fujitsu cross compiler
  IF (HIVE_BUILD_WITH_MPI)
    SET(CMAKE_C_COMPILER mpifccpx)
    SET(CMAKE_CXX_COMPILER mpiFCCpx)
  ELSE()
    SET(CMAKE_C_COMPILER fccpx)
    SET(CMAKE_CXX_COMPILER FCCpx)
  ENDIF()

  # Prevent CMake from adding GNU-specific linker flags (-rdynamic)
  SET(CMAKE_C_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C cross-compiler" FORCE)
  SET(CMAKE_CXX_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C++ cross-compiler" FORCE)

  # gcc compatible flag(-Xg)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xg")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xg")

  # PIC flag
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

  # OpenMP flag(-Kopenmp)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kopenmp")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kopenmp")

  IF (CMAKE_BUILD_TYPE MATCHES "Release")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast")
  ENDIF()

  IF (HIVE_BUILD_WITH_PMLIB)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Nfjcex -Ntl_notrt -DK_COMPUTER")
    SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Nfjcex -Ntl_notrt -DK_COMPUTER")
  ENDIF()

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
  
ELSE()
  IF (CMAKE_SYSTEM_PROCESSOR MATCHES "s64fx")
  ELSE () # assume gcc + linux or macosx
    # PIC flag
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

    IF (HIVE_BUILD_WITH_OPENMP)
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    ENDIF()
  ENDIF()
ENDIF()

# --------------------------------------
# MPI
IF(HIVE_BUILD_WITH_MPI)
  add_definitions(-DHIVE_ENABLE_MPI)
ENDIF()
# --------------------------------------

# Headers
set(INCLUDES
  ${PROJECT_SOURCE_DIR}/../third_party/lua/src
  ${CMAKE_BINARY_DIR}/third_party/lua/
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/include
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/gles
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/render
	${PROJECT_SOURCE_DIR}/../third_party/local
	${PROJECT_SOURCE_DIR}/../third_party/local/include
	${PROJECT_SOURCE_DIR}/../third_party/local/TextParser/include
	${CMAKE_SOURCE_DIR}/include
  
	${CMAKE_SOURCE_DIR}/include/Buffer
	${CMAKE_SOURCE_DIR}/include/Core
	${CMAKE_SOURCE_DIR}/include/Generator
	${CMAKE_SOURCE_DIR}/include/Image
	${CMAKE_SOURCE_DIR}/include/Renderer
	${CMAKE_SOURCE_DIR}/include/RenderObject
	${CMAKE_SOURCE_DIR}/include/SceneScript
  
  # for static link
	${PROJECT_SOURCE_DIR}/../HiveCore/src/Buffer
	${PROJECT_SOURCE_DIR}/../HiveCore/src/Core
	${PROJECT_SOURCE_DIR}/../HiveCore/src/Generator
	${PROJECT_SOURCE_DIR}/../HiveCore/src/Image
	${PROJECT_SOURCE_DIR}/../HiveCore/src/Renderer
	${PROJECT_SOURCE_DIR}/../HiveCore/src/RenderObject
	${PROJECT_SOURCE_DIR}/../HiveCore/src/SceneScript
  
  ${PROJECT_SOURCE_DIR}/../lib/Analyzer
	${PROJECT_SOURCE_DIR}/../lib/Builder
  ${PROJECT_SOURCE_DIR}/../lib/Builder/FloatsToFloat
  ${PROJECT_SOURCE_DIR}/../lib/Builder/ImageFilter
  ${PROJECT_SOURCE_DIR}/../lib/Builder/MeshAttribToVolume
  ${PROJECT_SOURCE_DIR}/../lib/Builder/PointToVolume
  ${PROJECT_SOURCE_DIR}/../lib/Builder/SolidAttribToVolume
  ${PROJECT_SOURCE_DIR}/../lib/Builder/SolidDfToVolume
  ${PROJECT_SOURCE_DIR}/../lib/Builder/SolidToVolume
  ${PROJECT_SOURCE_DIR}/../lib/Builder/SparseVolumeToVolume
  ${PROJECT_SOURCE_DIR}/../lib/Builder/VolumeFilter
  ${PROJECT_SOURCE_DIR}/../lib/Builder/VolumeQuantizer
  ${PROJECT_SOURCE_DIR}/../lib/Builder/VolumeToMeshData
  ${PROJECT_SOURCE_DIR}/../lib/Builder/VolumeToVector
  ${PROJECT_SOURCE_DIR}/../lib/ClusterParallelCoord
	${PROJECT_SOURCE_DIR}/../lib/Generator
  ${PROJECT_SOURCE_DIR}/../lib/Generator/MandelbulbVolGen
	${PROJECT_SOURCE_DIR}/../lib/Loader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/BcmLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/CdmLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/HdmLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/ObjLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/PdbLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/PdmLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/RawSparseVolumeLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/RawVolumeLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/SphLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/StlLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/UdmLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/VisioColorMapLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/VisioGradientMapLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/VolLoader
  ${PROJECT_SOURCE_DIR}/../lib/Loader/VtkLoader
  ${PROJECT_SOURCE_DIR}/../lib/Network
	${PROJECT_SOURCE_DIR}/../lib/Saver
  ${PROJECT_SOURCE_DIR}/../lib/Saver/PdmSaver
  ${PROJECT_SOURCE_DIR}/../lib/Saver/RawVolumeSaver
  ${PROJECT_SOURCE_DIR}/../lib/Saver/SphSaver
  ${PROJECT_SOURCE_DIR}/../lib/Saver/VisioColorMapSaver
  ${PROJECT_SOURCE_DIR}/../lib/Saver/VisioGradientMapSaver
  ${PROJECT_SOURCE_DIR}/../lib/Util/TpUtil
)
INCLUDE_DIRECTORIES(${INCLUDES})


# Source files
set(HRENDER_SOURCES
  src/main.cpp
  
  ../HiveCore/src/Core/Path.cpp
  ../HiveCore/src/Core/Perf.cpp
  ../HiveCore/src/Buffer/Buffer.cpp
  ../HiveCore/src/Buffer/BufferMeshData.cpp
  ../HiveCore/src/Buffer/BufferVolumeData.cpp
  ../HiveCore/src/Buffer/BufferLineData.cpp
  ../HiveCore/src/Buffer/BufferSparseVolumeData.cpp
  ../HiveCore/src/Buffer/BufferSolidData.cpp
  ../HiveCore/src/Buffer/BufferPointData.cpp
  ../HiveCore/src/Buffer/BufferVectorData.cpp
  ../HiveCore/src/Buffer/BufferTetraData.cpp
  ../HiveCore/src/Buffer/BufferImageData.cpp
  ../HiveCore/src/Buffer/BufferExtraData.cpp
  ../HiveCore/src/Buffer/UserBufferData.cpp
  ../HiveCore/src/Renderer/RenderCore.cpp
  ../HiveCore/src/Renderer/BaseBuffer.cpp
  ../HiveCore/src/Renderer/Commands_GL.cpp
  ../HiveCore/src/Renderer/Commands_LSGL.cpp
  ../HiveCore/src/Renderer/PolygonBuffer.cpp
  ../HiveCore/src/Renderer/VolumeBuffer.cpp
  ../HiveCore/src/Renderer/SparseVolumeBuffer.cpp
  ../HiveCore/src/Renderer/LineBuffer.cpp
  ../HiveCore/src/Renderer/PointBuffer.cpp
  ../HiveCore/src/Renderer/TetraBuffer.cpp
  ../HiveCore/src/Renderer/SolidBuffer.cpp
  ../HiveCore/src/Renderer/VectorBuffer.cpp
  ../HiveCore/src/SceneScript/SceneScript.cpp
  ../HiveCore/src/RenderObject/Camera.cpp
  ../HiveCore/src/RenderObject/RenderObject.cpp
  ../HiveCore/src/RenderObject/PolygonModel.cpp
  ../HiveCore/src/RenderObject/VolumeModel.cpp
  ../HiveCore/src/RenderObject/SparseVolumeModel.cpp
  ../HiveCore/src/RenderObject/LineModel.cpp
  ../HiveCore/src/RenderObject/PointModel.cpp
  ../HiveCore/src/RenderObject/TetraModel.cpp
  ../HiveCore/src/RenderObject/SolidModel.cpp
  ../HiveCore/src/RenderObject/VectorModel.cpp
  ../HiveCore/src/Image/jpge.cpp
  ../HiveCore/src/Image/jpgd.cpp
  ../HiveCore/src/Image/lodepng.cpp
  ../HiveCore/src/Image/tinyexr.cpp
  ../HiveCore/src/Image/ImageLoader.cpp
  ../HiveCore/src/Image/GenTexture.cpp
  ../HiveCore/src/Image/ImageSaver.cpp
  ../HiveCore/src/Generator/PrimitiveGenerator.cpp

  ../lib/Analyzer/PolygonAnalyzer.cpp
  ../lib/Analyzer/VolumeAnalyzer.cpp
  ../lib/Analyzer/SparseVolumeAnalyzer.cpp
  ../lib/Analyzer/SolidAnalyzer.cpp

  ../lib/Builder/FloatsToFloat/FloatsToFloat.cpp
  ../lib/Builder/MeshAttribToVolume/MeshAttribToVolume.cpp
  ../lib/Builder/PointToVolume/PointToVolume.cpp
  ../lib/Builder/SparseVolumeToVolume/SparseVolumeToVolume.cpp
  ../lib/Builder/VolumeFilter/CompileUtil.cpp
  ../lib/Builder/VolumeFilter/VolumeFilter.cpp
  ../lib/Builder/VolumeToMeshData/VolumeToMeshData.cpp
  ../lib/Builder/VolumeToVector/VolumeToVector.cpp
  ../lib/Builder/VolumeQuantizer/VolumeQuantizer.cpp
  ../lib/Builder/SolidToVolume/SolidToVolume.cpp
  ../lib/Builder/SolidDfToVolume/SolidDfToVolume.cpp
  ../lib/Builder/SolidAttribToVolume/SolidAttribToVolume.cpp
  ../lib/Builder/ImageFilter/ImageFilter.cpp

  ../lib/ClusterParallelCoord/VolumeClustering.cpp
  ../lib/ClusterParallelCoord/VolumeScatterPlot.cpp
  ../lib/Generator/MandelbulbVolGen/MandelbulbVolGen.cpp
  ../lib/Loader/ObjLoader/ObjLoader.cpp
  ../lib/Loader/PdbLoader/tiny_pdb.cpp
  ../lib/Loader/PdbLoader/PdbLoader.cpp
  ../lib/Loader/RawSparseVolumeLoader/RawSparseVolumeLoader.cpp
  ../lib/Loader/RawVolumeLoader/RawVolumeLoader.cpp
  ../lib/Loader/SphLoader/SphLoader.cpp
  ../lib/Loader/StlLoader/StlLoader.cpp
  ../lib/Loader/VisioColorMapLoader/VisioColorMapLoader.cpp
  ../lib/Loader/VisioGradientMapLoader/VisioGradientMapLoader.cpp
  ../lib/Loader/VolLoader/VolLoader.cpp
  ../lib/Loader/VtkLoader/tiny_vtkxml.cpp
  ../lib/Loader/VtkLoader/VtkLoader.cpp
  ../lib/Network/Connection.cpp
  ../lib/Network/easywsclient.cpp
  ../lib/Network/happyhttp.cpp
  ../lib/Network/MetaBinary.cpp
  ../lib/Saver/RawVolumeSaver/RawVolumeSaver.cpp
  ../lib/Saver/SphSaver/SphSaver.cpp
  ../lib/Saver/VisioColorMapSaver/VisioColorMapSaver.cpp
  ../lib/Saver/VisioGradientMapSaver/VisioGradientMapSaver.cpp
)

IF(HIVE_BUILD_WITH_CDMLIB)
  SET(HIVE_BUILD_WITH_BCMTOOLS On)
  add_definitions(-DHIVE_WITH_CDMLIB)

  IF(NOT HIVE_BUILD_WITH_MPI)
    add_definitions(-D_CDM_WITHOUT_MPI_)        # Use stub MPI defines(provided in CDMlib/include/mpi_stubs.h)
  ENDIF()

  SET(CDMLIB_SOURCES
    ../lib/Loader/CdmLoader/CdmLoader.cpp
  )

  IF (HIVE_BUILD_K_CROSS_COMPILE) # K specific option.
    SET(NETCDF_LIBS netcdf hdf5_hl hdf5 sz)
  ELSE()
    SET(NETCDF_LIBS netcdf hdf5_hl hdf5 z)
  ENDIF()

  IF(HIVE_BUILD_WITH_MPI)
    SET(CDMLIB_LIBS
      CDMmpi TPmpi ${NETCDF_LIBS}
    )
  ELSE()
    SET(CDMLIB_LIBS
      CDM TP ${NETCDF_LIBS}
    )
  ENDIF()

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/CDMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/CDMlib/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/lib)

  IF (HIVE_BUILD_K_CROSS_COMPILE) # K specific option.
    set(CMAKE_SHARED_LINKER_FLAGS -L/opt/aics/netcdf/k/lib-static)
    include_directories(/opt/aics/netcdf/k/include)
    link_directories(/opt/aics/netcdf/k/lib-static)
  ENDIF()
ENDIF() # HIVE_WITH_CDMLIB

IF(HIVE_BUILD_WITH_BCMTOOLS)
  add_definitions(-DHIVE_WITH_BCMTOOLS)

  # Additional Libraries
  if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      if(HIVE_BUILD_WITH_BCMTOOLS)
          add_definitions(-U__BLOCKS__) # work around for BCMlib's Block.h header
      endif()
  endif()

  SET(BCMTOOLS_SOURCES
    ../lib/Loader/BcmLoader/BcmLoader.cpp
  )

  SET(BCMTOOLS_LIBS
    BCM POLYmpi TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/Polylib/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/lib)
ENDIF() # HIVE_BUILD_WITH_BCMTOOLS

IF(HIVE_BUILD_WITH_HDMLIB)
  SET(HIVE_BUILD_WITH_BCMTOOLS On)
  add_definitions(-DHIVE_WITH_HDMLIB)

  # Source files
  SET(HDMLIB_SOURCES
    ../lib/Loader/HdmLoader/HdmLoader.cpp
  )
  SET(HDMLIB_LIBS
    HDM BCM TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/Polylib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/HDMlib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/HDMlib/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)
ENDIF() # HIVE_BUILD_WITH_HDMLIB

IF(HIVE_BUILD_WITH_PDMLIB)
  add_definitions(-DHIVE_WITH_PDMLIB)

  SET(PDMLIB_SOURCES
    ../lib/Loader/PdmLoader/PdmLoader.cpp
    ../lib/Saver/PdmSaver/PdmSaver.cpp
  )

  SET(PDMLIB_LIBS
    PDM TPmpi fpzip zoltan z
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/PDMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/PDMlib/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/lib)
ENDIF() # HIVE_BUILD_WITH_PDMLIB


IF(HIVE_BUILD_WITH_UDMLIB)
  add_definitions(-DHIVE_WITH_UDMLIB)

  SET(UDMLIB_SOURCES
  ../lib/Loader/UdmLoader/UdmLoader.cpp
  )

  SET(UDMLIB_LIBS
    udm TPmpi zoltan cgns
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/Polylib/include)
  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/UDMlib/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/UDMlib/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/lib)
ENDIF()

IF(HIVE_BUILD_WITH_COMPOSITOR)
  add_definitions(-DHIVE_WITH_COMPOSITOR)

  SET(COMPOSITOR_LIBS
    234comp
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/234Compositor/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/234Compositor/lib)
  
ENDIF()

IF(HIVE_BUILD_WITH_TPUTIL)
  add_definitions(-DHIVE_WITH_TPUTIL)

  SET(TPLIB_SOURCES
  ../lib/Util/TpUtil/TpUtil.cpp
  )

  SET(TPLIB_LIBS
    TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)

ENDIF() # HIVE_BUILD_WITH_TPUTIL


IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF()


IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF()

# --------- Ext lib ------------------
# PMlib
IF(HIVE_BUILD_WITH_PMLIB)
  #SET(PMLIB_LIBS
  #  PMmpi
  #)

  #include_directories(${CMAKE_SOURCE_DIR}/third_party/local/PMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/PMlib/lib)
ENDIF()

# Compositor

IF(HIVE_BUILD_WITH_COMPOSITOR)

  SET(COMPOSITOR_LIBS
    234comp
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/234Compositor/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/234Compositor/lib)
  
ENDIF()
# ---------------------------------------


# hrender
add_executable (hrender ${HRENDER_SOURCES}
    ${CDMLIB_SOURCES} ${BCMTOOLS_SOURCES} ${HDMLIB_SOURCES} ${PDMLIB_SOURCES} ${UDMLIB_SOURCES}
     ${TPLIB_SOURCES}
  )
add_sanitizers (hrender)

target_link_libraries(hrender pthread)
target_link_libraries(hrender liblua)
target_link_libraries(hrender LSGLES)
target_link_libraries(hrender ${CDMLIB_LIBS})
target_link_libraries(hrender ${HDMLIB_LIBS})
target_link_libraries(hrender ${UDMLIB_LIBS})
target_link_libraries(hrender ${PDMLIB_LIBS})
target_link_libraries(hrender ${BCMTOOLS_LIBS})
target_link_libraries(hrender ${COMPOSITOR_LIBS})
target_link_libraries(hrender ${TPLIB_LIBS})

# Link
if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  find_library(COCOA Cocoa)
  if (NOT COCOA)
    message(FATAL_ERROR "Cocoa Framework not found")
  endif()
  target_link_libraries(hrender ${COCOA})
  if (HIVE_BUILD_WITH_OPENGL)
    find_library(OPENGL OpenGL)
    if (NOT OPENGL)
      message(FATAL_ERROR "OpenGL Framework not found")
    endif()
    target_link_libraries(hrender ${OPENGL})
  endif()

endif()

if (HIVE_BUILD_WITH_OPENGL)
 if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  find_library(OPENGL OpenGL)
  if (NOT OPENGL)
    message(FATAL_ERROR "OpenGL Framework not found")
  endif()
  target_link_libraries(hrender ${OPENGL})
 else()
  find_library(OPENGL GL)
  if (NOT OPENGL)
    message(FATAL_ERROR "OpenGL Framework not found")
  endif()
  target_link_libraries(hrender ${OPENGL})
 endif()
endif()

# Additional Libraries
if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(HIVE_BUILD_WITH_BCMTOOLS)
        add_definitions(-U__BLOCKS__) # work around for BCMlib's Block.h header
    endif()
        find_library(COCOA Cocoa)
        if (NOT COCOA)
            message(FATAL_ERROR "Cocoa Framework not found")
        endif()
        target_link_libraries(hrender ${COCOA})
endif()

# Copy GLSL Compiler in post build phase
# @todo { Don't copy whole directory files }
SET(GLSLC_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/glsl)
ADD_CUSTOM_TARGET(glslc-copy-files ALL COMMAND cmake -E copy_directory
	${CMAKE_SOURCE_DIR}/third_party/SURFACE/glsl
	${EXECUTABLE_OUTPUT_PATH}/glsl)

# NOTE: COPY command only works at CMake configure phase
#file(COPY ${GLSL_PATH} DESTINATION ${EXECUTABLE_OUTPUT_PATH})

# ------------------
# [For develop]

# create symbolic link version (but can't work)
#ADD_CUSTOM_COMMAND(OUTPUT myLinkDebug COMMAND ln;-s;${EXECUTABLE_OUTPUT_PATH}/glsl;${EXECUTABLE_OUTPUT_PATH}/Debug/glsl)
#ADD_CUSTOM_COMMAND(OUTPUT myLinkRelease COMMAND ln;-s;${EXECUTABLE_OUTPUT_PATH}/glsl;${EXECUTABLE_OUTPUT_PATH}/Release/glsl)
#ADD_DEPENDENCIES(hrender myLinkDebug)
#ADD_DEPENDENCIES(hrender myLinkRelease)

# Alt. copy folders
file(COPY ${GLSL_PATH} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug)
file(COPY ${GLSL_PATH} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release)
# ------------------


#
# For INSTALL
#
install(TARGETS hrender DESTINATION bin)

#
# for TEST
#
add_test(NAME loader_obj.scn             COMMAND hrender  ../../hrender/test/loader_obj.scn           )
add_test(NAME loader_sph.scn             COMMAND hrender  ../../hrender/test/loader_sph.scn           )
add_test(NAME loader_cdm.scn             COMMAND hrender  ../../hrender/test/loader_cdm.scn           )
add_test(NAME loader_stl.scn             COMMAND hrender  ../../hrender/test/loader_stl.scn           )
add_test(NAME loader_pdb.scn             COMMAND hrender  ../../hrender/test/loader_pdb.scn           )
add_test(NAME loader_vol.scn             COMMAND hrender  ../../hrender/test/loader_vol.scn           )
add_test(NAME volume_to_vector.scn       COMMAND hrender  ../../hrender/test/volume_to_vector.scn     )
add_test(NAME render_basic.scn           COMMAND hrender  ../../hrender/test/render_basic.scn         )
add_test(NAME render_camera.scn          COMMAND hrender  ../../hrender/test/render_camera.scn        )
add_test(NAME render_camera2.scn         COMMAND hrender  ../../hrender/test/render_camera2.scn       )
add_test(NAME render_obj.scn             COMMAND hrender  ../../hrender/test/render_obj.scn           )
add_test(NAME render_sph.scn             COMMAND hrender  ../../hrender/test/render_sph.scn           )
add_test(NAME render_cdm.scn             COMMAND hrender  ../../hrender/test/render_cdm.scn           )
add_test(NAME render_vol.scn             COMMAND hrender  ../../hrender/test/render_vol.scn           )
add_test(NAME render_stl.scn             COMMAND hrender  ../../hrender/test/render_stl.scn           )
add_test(NAME render_pdb.scn             COMMAND hrender  ../../hrender/test/render_pdb.scn           )
add_test(NAME render_pbs.scn             COMMAND hrender  ../../hrender/test/render_pbs.scn           )
add_test(NAME render_pdm.scn             COMMAND hrender  ../../hrender/test/render_pdm.scn           )
add_test(NAME render_with_gentex.scn      COMMAND hrender ../../hrender/test/render_with_gentex.scn    )
add_test(NAME render_with_gentex_F32.scn      COMMAND hrender ../../hrender/test/render_with_gentex_F32.scn    )
add_test(NAME render_vol_gentex3d.scn      COMMAND hrender ../../hrender/test/render_vol_gentex3d.scn    )
add_test(NAME render_mirror_glass.scn      COMMAND hrender ../../hrender/test/render_mirror_glass.scn    )
add_test(NAME render_obj_point.scn       COMMAND hrender  ../../hrender/test/render_obj_point.scn     )
add_test(NAME render_obj_line.scn        COMMAND hrender  ../../hrender/test/render_obj_line.scn      )
add_test(NAME render_obj_normal_vector.scn      COMMAND hrender  ../../hrender/test/render_obj_normal_vector.scn)
add_test(NAME render_obj_extra.scn       COMMAND hrender  ../../hrender/test/render_obj_extra.scn )
add_test(NAME render_sph_vector.scn      COMMAND hrender  ../../hrender/test/render_sph_vector.scn    )
add_test(NAME render_with_texture.scn    COMMAND hrender  ../../hrender/test/render_with_texture.scn    )
add_test(NAME render_cube.scn            COMMAND hrender  ../../hrender/test/render_cube.scn    )
add_test(NAME render_pointlist.scn       COMMAND hrender  ../../hrender/test/render_pointlist.scn    )
add_test(NAME render_transform_obj.scn   COMMAND hrender  ../../hrender/test/render_obj_transform.scn )
add_test(NAME render_transform_sph.scn   COMMAND hrender  ../../hrender/test/render_sph_transform.scn )
#ERROR CASE
add_test(NAME loader_obj_err.scn         COMMAND hrender ../../hrender/test/err/loader_obj_err.scn           )
add_test(NAME loader_sph_err.scn         COMMAND hrender ../../hrender/test/err/loader_sph_err.scn           )
add_test(NAME loader_stl_err.scn         COMMAND hrender ../../hrender/test/err/loader_stl_err.scn           )
add_test(NAME loader_vol_err.scn         COMMAND hrender ../../hrender/test/err/loader_vol_err.scn           )
add_test(NAME render_obj_err.scn         COMMAND hrender ../../hrender/test/err/render_obj_err.scn           )
add_test(NAME render_sph_err.scn         COMMAND hrender ../../hrender/test/err/render_sph_err.scn           )

add_test(NAME builtin_functions.scn	COMMAND hrender ../../hrender/test/builtin_functions.scn)
add_test(NAME convert_pdb_to_pdm.scn	COMMAND hrender ../../hrender/test/convert_pdb_to_pdm.scn)
add_test(NAME get_table_test.scn	COMMAND hrender ../../hrender/test/get_table_test.scn)
add_test(NAME loader_saver_image.scn	COMMAND hrender ../../hrender/test/loader_saver_image.scn)
add_test(NAME point_to_volume.scn	COMMAND hrender ../../hrender/test/point_to_volume.scn)
add_test(NAME render_cornellbox.scn	COMMAND hrender ../../hrender/test/render_cornellbox.scn)
add_test(NAME render_float2float_sph.scn	COMMAND hrender ../../hrender/test/render_float2float_sph.scn)
add_test(NAME render_hdm.scn	COMMAND hrender ../../hrender/test/render_hdm.scn)
add_test(NAME render_ibl.scn	COMMAND hrender ../../hrender/test/render_ibl.scn)
add_test(NAME render_linelist.scn	COMMAND hrender ../../hrender/test/render_linelist.scn)
add_test(NAME render_maya.scn	COMMAND hrender ../../hrender/test/render_maya.scn)
add_test(NAME render_md.scn	COMMAND hrender ../../hrender/test/render_md.scn)
add_test(NAME render_obj_pathtrace.scn	COMMAND hrender ../../hrender/test/render_obj_pathtrace.scn)
add_test(NAME render_obj_screenparallel.scn	COMMAND hrender ../../hrender/test/render_obj_screenparallel.scn)
add_test(NAME render_pbs_reflection.scn	COMMAND hrender ../../hrender/test/render_pbs_reflection.scn)
add_test(NAME render_pbs_refraction.scn	COMMAND hrender ../../hrender/test/render_pbs_refraction.scn)
add_test(NAME render_pdb_elementsymbol_as_material.scn	COMMAND hrender ../../hrender/test/render_pdb_elementsymbol_as_material.scn)
add_test(NAME render_perlin.scn	COMMAND hrender ../../hrender/test/render_perlin.scn)
add_test(NAME render_primitiveAO.scn	COMMAND hrender ../../hrender/test/render_primitiveAO.scn)
add_test(NAME render_pvti.scn	COMMAND hrender ../../hrender/test/render_pvti.scn)
add_test(NAME render_quad.scn	COMMAND hrender ../../hrender/test/render_quad.scn)
add_test(NAME render_rawvolume.scn	COMMAND hrender ../../hrender/test/render_rawvolume.scn)
add_test(NAME render_sph_contour.scn	COMMAND hrender ../../hrender/test/render_sph_contour.scn)
add_test(NAME render_sph_isosurf.scn	COMMAND hrender ../../hrender/test/render_sph_isosurf.scn)
add_test(NAME render_sphere.scn	COMMAND hrender ../../hrender/test/render_sphere.scn)
add_test(NAME render_stl_point.scn	COMMAND hrender ../../hrender/test/render_stl_point.scn)
add_test(NAME render_teapot.scn	COMMAND hrender ../../hrender/test/render_teapot.scn)
add_test(NAME render_udm_tetra.scn	COMMAND hrender ../../hrender/test/render_udm_tetra.scn)
add_test(NAME render_vol_and_polygon.scn	COMMAND hrender ../../hrender/test/render_vol_and_polygon.scn)
add_test(NAME render_vol_cyc.scn	COMMAND hrender ../../hrender/test/render_vol_cyc.scn)
add_test(NAME render_vol_cyc2.scn	COMMAND hrender ../../hrender/test/render_vol_cyc2.scn)
add_test(NAME render_volume_cut_plane.scn	COMMAND hrender ../../hrender/test/render_volume_cut_plane.scn)
add_test(NAME render_volume_cut_plane_contour.scn	COMMAND hrender ../../hrender/test/render_volume_cut_plane_contour.scn)
add_test(NAME render_with_cyc.scn	COMMAND hrender ../../hrender/test/render_with_cyc.scn)
add_test(NAME render_with_exr.scn	COMMAND hrender ../../hrender/test/render_with_exr.scn)
add_test(NAME render_with_gentex1D.scn	COMMAND hrender ../../hrender/test/render_with_gentex1D.scn)
add_test(NAME saver_sph.scn	COMMAND hrender ../../hrender/test/saver_sph.scn)
add_test(NAME render_with_gentex3d.scn	COMMAND hrender ../../hrender/test/render_with_gentex3d.scn)
add_test(NAME render_with_gentex_RGBA32.scn	COMMAND hrender ../../hrender/test/render_with_gentex_RGBA32.scn)
add_test(NAME render_with_gentex_simple.scn	COMMAND hrender ../../hrender/test/render_with_gentex_simple.scn)
add_test(NAME render_with_texture_nofiltering.scn	COMMAND hrender ../../hrender/test/render_with_texture_nofiltering.scn)
add_test(NAME send_httpimage.scn	COMMAND hrender ../../hrender/test/send_httpimage.scn)
add_test(NAME send_httpjson.scn	COMMAND hrender ../../hrender/test/send_httpjson.scn)
add_test(NAME send_renderimage.scn	COMMAND hrender ../../hrender/test/send_renderimage.scn)
add_test(NAME send_wsbinary.scn	COMMAND hrender ../../hrender/test/send_wsbinary.scn)
add_test(NAME send_wsimage.scn	COMMAND hrender ../../hrender/test/send_wsimage.scn)
add_test(NAME volume_filter_expr.scn	COMMAND hrender ../../hrender/test/volume_filter_expr.scn)
add_test(NAME volume_filter_laplacian.scn	COMMAND hrender ../../hrender/test/volume_filter_laplacian.scn)
add_test(NAME volume_to_mesh.scn	COMMAND hrender ../../hrender/test/volume_to_mesh.scn)
add_test(NAME volume_to_mesh_layer.scn	COMMAND hrender ../../hrender/test/volume_to_mesh_layer.scn)
add_test(NAME test_webgl_scene.scn	COMMAND hrender ../../hrender/test/test_webgl_scene.scn)
