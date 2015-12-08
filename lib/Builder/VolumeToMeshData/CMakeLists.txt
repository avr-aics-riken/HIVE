# Headers
set(INCLUDES
	.
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/Core
	${CMAKE_SOURCE_DIR}/include/Buffer
        ${CMAKE_SOURCE_DIR}/include/SceneScript
        ${CMAKE_SOURCE_DIR}/third_party/lua/src
        ${CMAKE_BINARY_DIR}/third_party/lua

)
include_directories(${INCLUDES})

# Source files
set(SOURCES
	VolumeToMeshData.cpp
	LuaEntry.cpp
)
add_library(VolumeToMeshData SHARED ${SOURCES})
target_link_libraries(VolumeToMeshData hiveCore)
set_target_properties(VolumeToMeshData PROPERTIES PREFIX "")
 
