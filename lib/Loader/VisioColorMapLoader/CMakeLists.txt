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
	VisioColorMapLoader.cpp
	LuaEntry.cpp
)
add_library(VisioColorMapLoader ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(VisioColorMapLoader hiveCore)
set_target_properties(VisioColorMapLoader PROPERTIES PREFIX "")
 
