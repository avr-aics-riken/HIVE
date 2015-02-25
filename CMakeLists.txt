cmake_minimum_required(VERSION 2.8)

# cpack is already included in thrid_party/lua 
# INCLUDE(CPack)

enable_testing()

PROJECT(HIVE)

# Build options
SET(HIVE_BUILD_UNIT_TEST OFF CACHE BOOL "Build unit test?")
SET(HIVE_BUILD_K_CROSS_COMPILE OFF CACHE BOOL "Build for K/FX10 target with corss compiling?")
SET(HIVE_BUILD_WITH_MPI OFF CACHE BOOL "Build with MPI support?")
SET(HIVE_BUILD_SCREEN_PARALLEL OFF CACHE BOOL "Build with screen parallel support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_CDMLIB OFF CACHE BOOL "Build with CDMlib support?(Also need to enable HIVE_BUILD_WITH_MPI option)")
SET(HIVE_BUILD_WITH_PDMLIB OFF CACHE BOOL "Build with PDMlib support?")
SET(HIVE_BUILD_WITH_HDMLIB OFF CACHE BOOL "Build with HDMlib support?")
SET(HIVE_BUILD_WITH_BCMTOOLS OFF CACHE BOOL "Build with BCMTools support?(Also need to enable HIVE_BUILD_WITH_MPI option)")

# PROJECT OUTPUT SETTING
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# modules
add_subdirectory(third_party/SURFACE)
add_subdirectory(third_party/lua)

# standalone tools
add_subdirectory(tools)

# build
add_subdirectory(hrender)
add_subdirectory(HIVE)


# For cpack
set(CPACK_PACKAGE_NAME "HIVE")
set(CPACK_PACKAGE_VENDOR "RIKEN")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HIVE, Heterogeneously Integrated Visualization Environment.")
set(CPACK_PACKAGE_VERSION "0.9.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "0")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "CPack Component Example")
include(CPack)

