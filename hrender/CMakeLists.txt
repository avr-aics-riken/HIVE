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
	src/SceneScript/PolygonModel_Lua.cpp
	src/SceneScript/VolumeModel_Lua.cpp
	src/SceneScript/Camera_Lua.cpp
	src/SceneScript/ObjLoader_Lua.cpp
	src/SceneScript/StlLoader_Lua.cpp
	src/SceneScript/VolLoader_Lua.cpp
	src/SceneScript/SphLoader_Lua.cpp
	src/SceneScript/SceneScript.cpp
	src/SceneScript/PolygonModel_Lua.cpp
	src/SceneScript/BufferMeshData_Lua.cpp
	src/SceneScript/BufferVolumeData_Lua.cpp
	src/SceneScript/PolygonAnalyzer_Lua.cpp
	src/Renderer/RenderCore.cpp
	src/Renderer/BaseBuffer.cpp
	src/Renderer/PolygonBuffer.cpp
	src/Renderer/Commands_GL.cpp
	src/Renderer/Commands_LSGL.cpp
	src/Image/jpge.cpp
	src/Analyzer/PolygonAnalyzer.cpp
	src/Buffer/Buffer.cpp
	src/Buffer/BufferMeshData.cpp
	src/Buffer/BufferVolumeData.cpp
	src/Loader/Loader.cpp
	src/Loader/ObjLoader.cpp
	src/Loader/StlLoader.cpp
	src/Loader/VolLoader.cpp
	src/Loader/SphLoader.cpp
	src/RenderObject/Camera.cpp
	src/RenderObject/PolygonModel.cpp
	src/RenderObject/VolumeModel.cpp
)

add_executable (hrender ${SOURCES})

target_link_libraries(hrender liblua)
target_link_libraries(hrender LSGLES)


# Test
add_subdirectory(test)

