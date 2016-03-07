# Headers
set(INCLUDES
	.
	../../Loader/SphLoader
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
	SphSaver.cpp
	LuaEntry.cpp
)
add_library(SphSaver ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(SphSaver hiveCore)
set_target_properties(SphSaver PROPERTIES PREFIX "")

 
