cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers) # Address sanitizer.

IF(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/third_party/TextParser/src")
  MESSAGE(FATAL_ERROR "HIVE dependency repositories(TextParser, SURFACE, etc.) are missing!"
    "You probably did not clone the project with --recursive."
    " Plese call\n \"git submodule update --init\" first.")
ENDIF()

enable_testing()

PROJECT(HIVE)

# Build options
SET(HIVE_BUILD_UNIT_TEST OFF CACHE BOOL "Build unit test?")
SET(HIVE_BUILD_K_CROSS_COMPILE OFF CACHE BOOL "Build for K/FX10 target with corss compiling?")
SET(HIVE_BUILD_WITH_MPI OFF CACHE BOOL "Build with MPI support?")
SET(HIVE_BUILD_WITH_OPENMP OFF CACHE BOOL "Build with OpenMP support?")
SET(HIVE_BUILD_SCREEN_PARALLEL OFF CACHE BOOL "Build with screen parallel support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_CDMLIB OFF CACHE BOOL "Build with CDMlib support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_PDMLIB OFF CACHE BOOL "Build with PDMlib support?")
SET(HIVE_BUILD_WITH_HDMLIB OFF CACHE BOOL "Build with HDMlib support?")
SET(HIVE_BUILD_WITH_UDMLIB OFF CACHE BOOL "Build with UDMlib support?")
SET(HIVE_BUILD_WITH_BCMTOOLS OFF CACHE BOOL "Build with BCMTools support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_COMPOSITOR OFF CACHE BOOL "Build with 234Compositor support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_OPENGL ON CACHE BOOL "Build with OpenGL support?(Also need to enable HIVE_BUILD_WITH_OPENGL option)")
SET(HIVE_BUILD_WITH_NANOMSG ON CACHE BOOL "Build with Nanomsg support?(Also need to enable HIVE_BUILD_WITH_NANOMSG option)")
SET(HIVE_BUILD_WITH_PMLIB OFF CACHE BOOL "Build with PMlib support?")
SET(HIVE_BUILD_WITH_CPMLIB OFF CACHE BOOL "Build with CPMlib support?")

# PROJECT OUTPUT SETTING
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# modules
IF(HIVE_BUILD_K_CROSS_COMPILE)
  SET(SURFACE_BUILD_K_CROSS_COMPILE ON CACHE BOOL "Force enable K/FX10 cross
  compiling for SURFACE")
ENDIF()
IF(HIVE_BUILD_WITH_MPI)
  SET(SURFACE_BUILD_WITH_MPI ON CACHE BOOL "Force enable MPI for SURFACE")
ENDIF()
IF(HIVE_BUILD_WITH_OPENMP)
  SET(SURFACE_BUILD_WITH_OPENMP ON CACHE BOOL "Force enable OpenMP for
  SURFACE" FORCE)
ENDIF()
add_subdirectory(third_party/SURFACE)
add_subdirectory(third_party/lua)

# standalone tools
add_subdirectory(tools)

# build
add_subdirectory(HiveCore)
add_subdirectory(lib)
add_subdirectory(hrender)
add_subdirectory(HIVE)

# Extract Electron
execute_process(
	COMMAND cmake -E tar xf ${CMAKE_SOURCE_DIR}/third_party/electron.zip
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
