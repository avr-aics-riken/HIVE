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
	VolLoader.cpp
	LuaEntry.cpp
)
add_library(VolLoader SHARED ${SOURCES})
target_link_libraries(VolLoader hiveCore)
set_target_properties(VolLoader PROPERTIES PREFIX "")
 
