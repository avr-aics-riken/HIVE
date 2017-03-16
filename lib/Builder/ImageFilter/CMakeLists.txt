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
	ImageFilter.cpp
	LuaEntry.cpp
)
add_library(ImageFilter ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(ImageFilter hiveCore)
target_link_libraries(ImageFilter liblua)
set_target_properties(ImageFilter PROPERTIES PREFIX "")
 
