cmake_minimum_required(VERSION 2.8)

enable_testing()

PROJECT(HIVE)

# -- Read version from git tag -----------------------------------------------
# Based on https://github.com/lcw/cmake_git_version/blob/master/CMakeLists.txt
FIND_PACKAGE(Git)

IF(GIT_FOUND)
  SET(USE_GIT 1)
ELSE(GIT_FOUND)
  SET(USE_GIT 0)
ENDIF(GIT_FOUND)

FILE(WRITE ${CMAKE_BINARY_DIR}/version.h.in
"\#define VERSION \"@VERSION@\"\n"
)

FILE(WRITE ${CMAKE_BINARY_DIR}/Version.cmake
"IF(${USE_GIT} EQUAL 1)
  EXECUTE_PROCESS(
       COMMAND ${GIT_EXECUTABLE} describe --tags --dirty
       WORKING_DIRECTORY \${WORKING_DIRECTORY}
       OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
       RESULT_VARIABLE GIT_DESCRIBE_RESULT
       ERROR_VARIABLE GIT_DESCRIBE_ERROR
       OUTPUT_STRIP_TRAILING_WHITESPACE
   )
ELSE(${USE_GIT} EQUAL 1)
  SET(GIT_DESCRIBE_RESULT -1)
ENDIF(${USE_GIT} EQUAL 1)

IF(GIT_DESCRIBE_RESULT EQUAL 0)
  FILE(WRITE ${CMAKE_BINARY_DIR}/source/VERSION \"\${GIT_DESCRIBE_VERSION}\")
ELSE(GIT_DESCRIBE_RESULT EQUAL 0)
  FILE(READ  ${CMAKE_SOURCE_DIR}/VERSION GIT_DESCRIBE_VERSION)
ENDIF(GIT_DESCRIBE_RESULT EQUAL 0)
STRING(REGEX REPLACE \"v([0-9]*)\\\\.([0-9]*)\\\\.(.*)\"
       \"\\\\1.\\\\2.\\\\3\" VERSION \"\${GIT_DESCRIBE_VERSION}\" )
STRING(REGEX REPLACE \"v([0-9]*)\\\\.([0-9]*)\\\\.(.*)\"
       \"\\\\1\" VERSION_MAJOR \"\${GIT_DESCRIBE_VERSION}\")
STRING(REGEX REPLACE \"v([0-9]*)\\\\.([0-9]*)\\\\.(.*)\"
       \"\\\\2\" VERSION_MINOR \"\${GIT_DESCRIBE_VERSION}\")
STRING(REGEX REPLACE \"v([0-9]*)\\\\.([0-9]*)\\\\.(.*)\"
       \"\\\\3\" VERSION_PATCH \"\${GIT_DESCRIBE_VERSION}\")
")

FILE(WRITE ${CMAKE_BINARY_DIR}/FillVersion.cmake
"INCLUDE(Version.cmake)
CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
")
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

ADD_CUSTOM_TARGET(
    version
    ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/version.h.in
                     -D DST=${CMAKE_BINARY_DIR}/version.h
                     -D WORKING_DIRECTORY=${CMAKE_SOURCE_DIR}
                     -P ${CMAKE_BINARY_DIR}/FillVersion.cmake
)
# -----------------------------------------------------------


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


# For cpack
SET(WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
INCLUDE(${CMAKE_BINARY_DIR}/Version.cmake)

set(CPACK_PACKAGE_NAME "HIVE")
set(CPACK_PACKAGE_VENDOR "RIKEN")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HIVE, Heterogeneously Integrated Visualization Environment.")
#set(CPACK_PACKAGE_VERSION "0.9.4")
#set(CPACK_PACKAGE_VERSION_MAJOR "0")
#set(CPACK_PACKAGE_VERSION_MINOR "9")
#set(CPACK_PACKAGE_VERSION_PATCH "4")

# Use version from git tag.
SET(CPACK_PACKAGE_VERSION "${VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VERSIONHEADER-${VERSIONHEADER_VERSION_FULL}")

set(CPACK_BUNDLE_NAME "HIVE")
set(CPACK_SOURCE_IGNORE_FILES
"~$"
".sw[a-z]$"
".DS_Store"
"tags"
"cscope.*"
"^${CMAKE_SOURCE_DIR}/.git.*"
)

# set(CPACK_PACKAGE_INSTALL_DIRECTORY "CPack Component Example")
include(CPack)

IF (UNIX)
  FILE(WRITE ${CMAKE_BINARY_DIR}/Dist.cmake
"MESSAGE(STATUS \"Package Dist running...\")
INCLUDE(CPackSourceConfig.cmake)
EXECUTE_PROCESS(COMMAND cpack -G TGZ --config CPackSourceConfig.cmake
  TIMEOUT 3600
  WORKING_DIRECTORY \${CMAKE_BINARY_DIR})
")
  #
  # Add custom target
  #
  ADD_CUSTOM_TARGET(dist
    COMMAND ${CMAKE_COMMAND} .
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/Dist.cmake
    )
ENDIF(UNIX)

ADD_DEPENDENCIES(hrender version)

