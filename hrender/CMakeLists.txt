cmake_minimum_required(VERSION 2.8)

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)

message(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})


# Headers
set(INCLUDES
	${PROJECT_SOURCE_DIR}/../third_party/lua/src
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/include
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/gles
	${PROJECT_SOURCE_DIR}/../hrender/src/Analyzer
	${PROJECT_SOURCE_DIR}/../hrender/src/Buffer
	${PROJECT_SOURCE_DIR}/../hrender/src/Builder
	${PROJECT_SOURCE_DIR}/../hrender/src/Loader
	${PROJECT_SOURCE_DIR}/../hrender/src/RenderObject
	${PROJECT_SOURCE_DIR}/../hrender/src/SceneScript
	${PROJECT_SOURCE_DIR}/../hrender/src/Core
	${CMAKE_BINARY_DIR}/third_party/lua
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
	src/Buffer/Buffer.cpp
	src/Buffer/BufferMeshData.cpp
	src/Buffer/BufferVolumeData.cpp
	src/Builder/FloatsToFloat.cpp
	src/Image/jpge.cpp
	src/Loader/Loader.cpp
	src/Loader/ObjLoader.cpp
	src/Loader/SphLoader.cpp
	src/Loader/StlLoader.cpp
	src/Loader/VolLoader.cpp
	src/RenderObject/Camera.cpp
	src/RenderObject/PolygonModel.cpp
	src/RenderObject/VolumeModel.cpp
	src/Renderer/BaseBuffer.cpp
	src/Renderer/Commands_GL.cpp
	src/Renderer/Commands_LSGL.cpp
	src/Renderer/PolygonBuffer.cpp
	src/Renderer/RenderCore.cpp
	src/SceneScript/SceneScript.cpp
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable (hrender ${SOURCES})

target_link_libraries(hrender liblua)
target_link_libraries(hrender LSGLES)

# Additional Libraries
if (APPLE)
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

# Test
add_subdirectory(test)

