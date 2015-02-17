cmake_minimum_required(VERSION 2.8)
enable_testing()

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)

# ------------------------------------------------------------
# Build options
SET(HIVE_BUILD_UNIT_TEST OFF CACHE BOOL "Build unit test?")
SET(HIVE_BUILD_K_CROSS_COMPILE OFF CACHE BOOL "Build for K/FX10 target with corss compiling?")
SET(HIVE_BUILD_WITH_MPI OFF CACHE BOOL "Build with MPI support?")
SET(HIVE_BUILD_SCREEN_PARALLEL OFF CACHE BOOL "Build with screen parallel support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_CDMLIB OFF CACHE BOOL "Build with CDMlib support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_PDMLIB OFF CACHE BOOL "Build with PDMlib support?")
SET(HIVE_BUILD_WITH_HDMLIB OFF CACHE BOOL "Build with HDMlib support?")
SET(HIVE_BUILD_WITH_BCMTOOLS OFF CACHE BOOL "Build with BCMTools support?(Also need to enable HIVE_BUILD_WITH_MPI option)")

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
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/include
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/gles
	${PROJECT_SOURCE_DIR}/../third_party/local/include
	${PROJECT_SOURCE_DIR}/../hrender/src/Analyzer
	${PROJECT_SOURCE_DIR}/../hrender/src/Buffer
	${PROJECT_SOURCE_DIR}/../hrender/src/Builder
	${PROJECT_SOURCE_DIR}/../hrender/src/Loader
	${PROJECT_SOURCE_DIR}/../hrender/src/Network
	${PROJECT_SOURCE_DIR}/../hrender/src/RenderObject
	${PROJECT_SOURCE_DIR}/../hrender/src/SceneScript
	${PROJECT_SOURCE_DIR}/../hrender/src/Core
    ${CMAKE_BINARY_DIR}/third_party/lua
    ${PROJECT_SOURCE_DIR}/../third_party/local/include
)
INCLUDE_DIRECTORIES(${INCLUDES})


# Links
set(LIBS
	${CMAKE_BINARY_DIR}/third_party/lua
)
link_directories(${LIBS})


# Source files
set(SOURCES
	src/Core/main.cpp
	src/Core/Path.cpp
	src/Analyzer/PolygonAnalyzer.cpp
	src/Analyzer/VolumeAnalyzer.cpp
	src/Buffer/Buffer.cpp
	src/Buffer/BufferMeshData.cpp
	src/Buffer/BufferVolumeData.cpp
	src/Buffer/BufferLineData.cpp
	src/Buffer/BufferSparseVolumeData.cpp
	src/Buffer/BufferPointData.cpp
	src/Buffer/BufferImageData.cpp
	src/Builder/FloatsToFloat.cpp
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
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/lib)
  
ENDIF()

IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF()

add_executable (hrender ${SOURCES} ${CDMLIB_SOURCES})

target_link_libraries(hrender liblua)
target_link_libraries(hrender LSGLES)
target_link_libraries(hrender ${CDMLIB_LIBS})


# Additional Libraries
if (APPLE)
    add_definitions(-D__BLOCKS__=0)
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

