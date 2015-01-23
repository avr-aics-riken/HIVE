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
	${PROJECT_SOURCE_DIR}/../hrender/src/Buffer
	${PROJECT_SOURCE_DIR}/../hrender/src/Loader
	${PROJECT_SOURCE_DIR}/../hrender/src/RenderObject
	${PROJECT_SOURCE_DIR}/../hrender/src/SceneScript
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
	src/SceneScript/PolygonModel.cpp
	src/SceneScript/SceneScript.cpp
	src/Renderer/RenderCore.cpp
	src/Renderer/Commands_GL.cpp
	src/Renderer/Commands_LSGL.cpp
	src/Image/jpge.cpp
	src/Buffer/Buffer.cpp
	src/Loader/Loader.cpp
	src/RenderObject/Camera.cpp
)

add_executable (hrender ${SOURCES})

target_link_libraries(hrender liblua)
target_link_libraries(hrender LSGLES)


# Test
add_subdirectory(test)

