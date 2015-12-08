cmake_minimum_required(VERSION 2.8)
enable_testing()

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

  # Prevent CMake from adding GNU-specific linker flags (-rdynamic)
  SET(CMAKE_C_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C cross-compiler" FORCE)
  SET(CMAKE_CXX_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C++ cross-compiler" FORCE)

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
ENDIF()

# --------------------------------------
# third party lib flags
#
IF(HIVE_BUILD_WITH_COMPOSITOR)
  IF(NOT HIVE_BUILD_WITH_MPI)
    message(FATAL_ERROR "HIVE_BUILD_WITH_MPI must be defined when HIVE_BUILD_WITH_COMPOSIOR is enabled")
  ENDIF()
  add_definitions(-DHIVE_WITH_COMPOSITOR)
ENDIF()
# --------------------------------------

# --------------------------------------
# MPI
IF(HIVE_BUILD_WITH_MPI)
  add_definitions(-DHIVE_ENABLE_MPI)
ENDIF()
# --------------------------------------

set(THIRD_INCLUDES
        ${CMAKE_SOURCE_DIR}/third_party/SURFACE/include
        ${CMAKE_SOURCE_DIR}/third_party/SURFACE/gles
        ${CMAKE_SOURCE_DIR}/third_party/SURFACE/render
        ${CMAKE_SOURCE_DIR}/third_party/local
        ${CMAKE_SOURCE_DIR}/third_party/local/include
        ${CMAKE_SOURCE_DIR}/third_party/local/TextParser/include
)
INCLUDE_DIRECTORIES(${THIRD_INCLUDES})

set(LIBS
        ${CMAKE_SOURCE_DIR}/third_party/lua
        ${CMAKE_SOURCE_DIR}/third_party/local/lib          # fpzip, zoltan
        ${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib
)
link_directories(${LIBS})

add_subdirectory(Analyzer)
add_subdirectory(Network)
add_subdirectory(Generator)
add_subdirectory(Loader)
add_subdirectory(Saver)
add_subdirectory(Builder)

