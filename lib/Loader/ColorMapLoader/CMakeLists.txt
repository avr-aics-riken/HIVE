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
	ColorMapLoader.cpp
	LuaEntry.cpp
)
add_library(ColorMapLoader ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(ColorMapLoader hiveCore)
set_target_properties(ColorMapLoader PROPERTIES PREFIX "")
 
