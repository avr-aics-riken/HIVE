cmake_minimum_required(VERSION 2.8)
enable_testing()

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)

# ------------------------------------------------------------

message(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})

# CDMlib depends on BCMTOOLS
IF(HIVE_BUILD_WITH_CDMLIB)
  SET(HIVE_BUILD_WITH_BCMTOOLS On)
ENDIF()

# --------------------------------------
# 
#  SAME Build Option with SURFACE
#
# Build options
#
# K/FX10 native environment
IF (CMAKE_SYSTEM_PROCESSOR MATCHES "s64fx")

  # Use native Fujitsu compiler
  IF (SURFACE_BUILD_WITH_MPI)
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

  # Prevent CMake from adding GNU-specific linker flags (-rdynamic)
  SET(CMAKE_C_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C cross-compiler" FORCE)
  SET(CMAKE_CXX_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C++ cross-compiler" FORCE)

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
ENDIF()

# K cross compiling
IF (SURFACE_BUILD_K_CROSS_COMPILE)

  # Use Fujitsu cross compiler
  IF (SURFACE_BUILD_WITH_MPI)
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

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
  
ENDIF()
# --------------------------------------

# --------------------------------------
# third party lib flags
#
IF(HIVE_BUILD_WITH_CDMLIB)
  add_definitions(-DHIVE_WITH_CDMLIB)
ENDIF()

IF(HIVE_BUILD_WITH_BCMTOOLS)
  add_definitions(-DHIVE_WITH_BCMTOOLS)
ENDIF()

IF(HIVE_BUILD_WITH_PDMLIB)
  add_definitions(-DHIVE_WITH_PDMLIB)
ENDIF()

IF(HIVE_BUILD_WITH_HDMLIB)
  add_definitions(-DHIVE_WITH_HDMLIB)
ENDIF()
# --------------------------------------

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
	${PROJECT_SOURCE_DIR}/../hrender/src/Analyzer
	${PROJECT_SOURCE_DIR}/../hrender/src/Buffer
	${PROJECT_SOURCE_DIR}/../hrender/src/Builder
	${PROJECT_SOURCE_DIR}/../hrender/src/Loader
	${PROJECT_SOURCE_DIR}/../hrender/src/Saver
	${PROJECT_SOURCE_DIR}/../hrender/src/Network
	${PROJECT_SOURCE_DIR}/../hrender/src/RenderObject
	${PROJECT_SOURCE_DIR}/../hrender/src/SceneScript
	${PROJECT_SOURCE_DIR}/../hrender/src/Core
)
INCLUDE_DIRECTORIES(${INCLUDES})


# Links
set(LIBS
	${CMAKE_BINARY_DIR}/third_party/lua
	${PROJECT_SOURCE_DIR}/../third_party/local/lib		# fpzip, zoltan
	${PROJECT_SOURCE_DIR}/../third_party/local/TextParser/lib
)
link_directories(${LIBS})


# Source files
set(SOURCES
	src/Core/main.cpp
	src/Core/Path.cpp
	src/Analyzer/PolygonAnalyzer.cpp
	src/Analyzer/VolumeAnalyzer.cpp
	src/Buffer/Buffer.cpp
	src/Buffer/BufferTetraData.cpp
	src/Buffer/BufferMeshData.cpp
	src/Buffer/BufferVolumeData.cpp
	src/Buffer/BufferLineData.cpp
	src/Buffer/BufferSparseVolumeData.cpp
	src/Buffer/BufferPointData.cpp
	src/Buffer/BufferImageData.cpp
	src/Builder/FloatsToFloat.cpp
	src/Builder/VolumeToMeshData.cpp
	src/Builder/VolumeToVector.cpp
	src/Image/jpge.cpp
	src/Image/ImageLoader.cpp
	src/Image/ImageSaver.cpp
	src/Loader/ObjLoader.cpp
	src/Loader/SphLoader.cpp
	src/Loader/StlLoader.cpp
	src/Loader/VolLoader.cpp
	src/Loader/PdbLoader.cpp
	src/Loader/tiny_pdb.cpp
	src/Network/Connection.cpp
	src/Network/easywsclient.cpp
	src/Network/happyhttp.cpp
	src/RenderObject/Camera.cpp
	src/RenderObject/PolygonModel.cpp
	src/RenderObject/VolumeModel.cpp
	src/RenderObject/LineModel.cpp
	src/RenderObject/PointModel.cpp
	src/Renderer/BaseBuffer.cpp
	src/Renderer/Commands_GL.cpp
	src/Renderer/Commands_LSGL.cpp
	src/Renderer/PolygonBuffer.cpp
	src/Renderer/VolumeBuffer.cpp
	src/Renderer/LineBuffer.cpp
	src/Renderer/PointBuffer.cpp
	src/Renderer/RenderCore.cpp
	src/SceneScript/SceneScript.cpp
	src/SceneScript/SceneScript.cpp
)

IF(HIVE_BUILD_WITH_CDMLIB)

  IF(NOT HIVE_BUILD_WITH_MPI)
    add_definitions(-D_CDM_WITHOUT_MPI_)	# Use stub MPI defines(provided in CDMlib/include/mpi_stubs.h)
  ENDIF()

  SET(CDMLIB_SOURCES
    src/Loader/CdmLoader.cpp
  )

  SET(CDMLIB_LIBS
    CDM TP
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/CDMlib/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/CDMlib/lib)
  
ENDIF()

IF(HIVE_BUILD_WITH_BCMTOOLS)

  SET(BCMTOOLS_SOURCES
    src/Loader/BcmLoader.cpp
  )

  SET(BCMTOOLS_LIBS
    BCM TP
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/BCMTools/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/BCMTools/lib)
  
ENDIF()

IF(HIVE_BUILD_WITH_HDMLIB)

  SET(HDMLIB_SOURCES
    src/Loader/HdmLoader.cpp
  )

  SET(HDMLIB_LIBS
    HDM TP
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/Polylib/include)
  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/HDMlib/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/HDMlib/lib)
  
ENDIF()

IF(HIVE_BUILD_WITH_PDMLIB)

  SET(PDMLIB_SOURCES
    src/Loader/PdmLoader.cpp
    src/Saver/PdmSaver.cpp
  )

  SET(PDMLIB_LIBS
    PDM TP fpzip zoltan z
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/PDMlib/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/PDMlib/lib)
  
ENDIF()

IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF()

add_executable (hrender ${SOURCES} ${CDMLIB_SOURCES} ${BCMTOOLS_SOURCES}
	${HDMLIB_SOURCES} ${PDMLIB_SOURCES} )

target_link_libraries(hrender liblua)
target_link_libraries(hrender LSGLES)
target_link_libraries(hrender ${CDMLIB_LIBS})
target_link_libraries(hrender ${HDMLIB_LIBS})
target_link_libraries(hrender ${PDMLIB_LIBS})
target_link_libraries(hrender ${BCMTOOLS_LIBS})


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



# Copy GLSL Compiler
SET(GLSL_PATH ${PROJECT_SOURCE_DIR}/../third_party/SURFACE/glsl)
file(COPY ${GLSL_PATH} DESTINATION ${EXECUTABLE_OUTPUT_PATH})

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
# for TEST
#
add_test(NAME loader_obj.scn             COMMAND hrender  ../../hrender/test/loader_obj.scn           )
add_test(NAME loader_sph.scn             COMMAND hrender  ../../hrender/test/loader_sph.scn           )
add_test(NAME loader_cdm.scn             COMMAND hrender  ../../hrender/test/loader_cdm.scn           )
add_test(NAME loader_stl.scn             COMMAND hrender  ../../hrender/test/loader_stl.scn           )
add_test(NAME loader_pdb.scn             COMMAND hrender  ../../hrender/test/loader_pdb.scn           )
add_test(NAME loader_vol.scn             COMMAND hrender  ../../hrender/test/loader_vol.scn           )
add_test(NAME render_basic.scn           COMMAND hrender  ../../hrender/test/render_basic.scn         )
add_test(NAME render_camera.scn          COMMAND hrender  ../../hrender/test/render_camera.scn        )
add_test(NAME render_camera2.scn         COMMAND hrender  ../../hrender/test/render_camera2.scn       )
add_test(NAME render_obj.scn             COMMAND hrender  ../../hrender/test/render_obj.scn           )
add_test(NAME render_sph.scn             COMMAND hrender  ../../hrender/test/render_sph.scn           )
add_test(NAME render_cdm.scn             COMMAND hrender  ../../hrender/test/render_cdm.scn           )
add_test(NAME render_vol.scn             COMMAND hrender  ../../hrender/test/render_vol.scn           )
add_test(NAME render_stl.scn             COMMAND hrender  ../../hrender/test/render_stl.scn           )
add_test(NAME render_pdb.scn             COMMAND hrender  ../../hrender/test/render_pdb.scn           )
add_test(NAME render_transform_obj.scn   COMMAND hrender  ../../hrender/test/render_obj_transform.scn )
add_test(NAME render_transform_sph.scn   COMMAND hrender  ../../hrender/test/render_sph_transform.scn )
add_test(NAME render_obj_point.scn       COMMAND hrender  ../../hrender/test/render_obj_point.scn     )
add_test(NAME render_obj_line.scn        COMMAND hrender  ../../hrender/test/render_obj_line.scn      )

#ERROR CASE
add_test(NAME loader_obj_err.scn         COMMAND hrender ../../hrender/test/err/loader_obj_err.scn           )
add_test(NAME loader_sph_err.scn         COMMAND hrender ../../hrender/test/err/loader_sph_err.scn           )
add_test(NAME loader_stl_err.scn         COMMAND hrender ../../hrender/test/err/loader_stl_err.scn           )
add_test(NAME loader_vol_err.scn         COMMAND hrender ../../hrender/test/err/loader_vol_err.scn           )
add_test(NAME render_obj_err.scn         COMMAND hrender ../../hrender/test/err/render_obj_err.scn           )
add_test(NAME render_sph_err.scn         COMMAND hrender ../../hrender/test/err/render_sph_err.scn           )

