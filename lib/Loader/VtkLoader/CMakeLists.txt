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
	VtkLoader.cpp
	LuaEntry.cpp
	tiny_vtkxml.cpp
)
add_library(VtkLoader ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(VtkLoader hiveCore)
set_target_properties(VtkLoader PROPERTIES PREFIX "")
 
