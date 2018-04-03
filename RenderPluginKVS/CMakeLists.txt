cmake_minimum_required(VERSION 2.8)
enable_testing()

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)

# ------------------------------------------------------------

message(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})

# --------------------------------------
# third party lib flags
#

IF(HIVE_BUILD_WITH_COMPOSITOR)
  IF(NOT HIVE_BUILD_WITH_MPI)
    message(FATAL_ERROR "HIVE_BUILD_WITH_MPI must be defined when HIVE_BUILD_WITH_COMPOSIOR is enabled")
  ENDIF()
  add_definitions(-DHIVE_WITH_COMPOSITOR)
ENDIF()

IF(HIVE_BUILD_WITH_PMLIB)
  IF(NOT HIVE_BUILD_WITH_MPI)
    message(FATAL_ERROR "HIVE_BUILD_WITH_MPI must be defined when HIVE_BUILD_WITH_PMLIB is enabled")
  ENDIF()
  add_definitions(-DHIVE_WITH_PMLIB)
ENDIF()
# --------------------------------------

# --------------------------------------
# MPI
IF(HIVE_BUILD_WITH_MPI)
  add_definitions(-DHIVE_ENABLE_MPI)
ENDIF()
# --------------------------------------

# --------------------------------------
# OpenGL
IF(HIVE_BUILD_WITH_OPENGL)
  add_definitions(-DHIVE_BUILD_WITH_OPENGL)
ENDIF()
# --------------------------------------

# GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(GLUT REQUIRED)
if (GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIRS})
    link_libraries(${GLUT_LIBRARIES})
endif()

# --------------------------------------

# --------------------------------------
# KVS

add_definitions(-DKVS_ENABLE_OPENGL=1)

if (KVS_FOUND)
    include_directories(${KVS_INCLUDE_DIRS})
    link_libraries(${KVS_LIBRARIES})
endif()

# --------------------------------------

# Headers
set(INCLUDES
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/third_party/lua/
	${CMAKE_SOURCE_DIR}/third_party/LUA/src
	${CMAKE_SOURCE_DIR}/third_party/local/kvs/include
	${CMAKE_SOURCE_DIR}/include/Buffer
	${CMAKE_SOURCE_DIR}/include/Network
	${CMAKE_SOURCE_DIR}/include/RenderObject
	${CMAKE_SOURCE_DIR}/include/Renderer
	${CMAKE_SOURCE_DIR}/include/SceneScript
	${CMAKE_SOURCE_DIR}/include/Core
	${CMAKE_SOURCE_DIR}/include/Image
	${CMAKE_SOURCE_DIR}/include/Generator
	${CMAKE_SOURCE_DIR}/include/Renderer  
)
INCLUDE_DIRECTORIES(${INCLUDES})


# Links
set(LIBS
	${CMAKE_BINARY_DIR}/third_party/lua
  ${CMAKE_SOURCE_DIR}/third_party/local/kvs/lib
)
link_directories(${LIBS})


# Source files

set(RENDERPLUGIN_SOURCES
  src/main.cpp
  src/RenderCoreKVS.cpp
)

IF(HIVE_BUILD_WITH_PMLIB)
  SET(PMLIB_LIBS
    PMmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/PMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/PMlib/lib)
ENDIF()

IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF()

IF(BUILD_SHARED_LIBS)
  SET(HIVE_LIBRARY_TYPE SHARED)
ELSE()
  add_definitions(-DHIVE_STATIC)
ENDIF()

# Render Plugin(SURFACE/OPENGL)
add_library (RenderPluginKVS ${HIVE_LIBRARY_TYPE} ${RENDERPLUGIN_SOURCES})

# Enable address sanitizer(if available).
add_sanitizers(RenderPluginKVS)


# link KVS lib
target_link_libraries(RenderPluginKVS kvsCore)
target_link_libraries(RenderPluginKVS kvsSupportGLUT)

# link HiveCore lib
target_link_libraries(RenderPluginKVS hiveCore)

# Link
if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  find_library(COCOA Cocoa)
  if (NOT COCOA)
    message(FATAL_ERROR "Cocoa Framework not found")
  endif()
  target_link_libraries(RenderPluginKVS ${COCOA})
  if (HIVE_BUILD_WITH_OPENGL)
    find_library(OPENGL OpenGL)
    if (NOT OPENGL)
      message(FATAL_ERROR "OpenGL Framework not found")
    endif()
    target_link_libraries(RenderPluginKVS ${OPENGL})
  endif()

endif()

if (HIVE_BUILD_WITH_OPENGL)
 if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  find_library(OPENGL OpenGL)
  if (NOT OPENGL)
    message(FATAL_ERROR "OpenGL Framework not found")
  endif()
  target_link_libraries(RenderPluginKVS ${OPENGL})
 else()
  find_library(OPENGL GL)
  if (NOT OPENGL)
    message(FATAL_ERROR "OpenGL Framework not found")
  endif()
  target_link_libraries(RenderPluginKVS ${OPENGL})
 endif()
endif()

target_link_libraries(RenderPluginKVS pthread)
#target_link_libraries(RenderPluginKVS LSGLES)
set_target_properties(RenderPluginKVS PROPERTIES PREFIX "")
