# Headers
set(INCLUDES
	.
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/Core
	${CMAKE_SOURCE_DIR}/include/Buffer
	${CMAKE_SOURCE_DIR}/HiveCore/src
        ${CMAKE_SOURCE_DIR}/include/SceneScript
        ${CMAKE_SOURCE_DIR}/third_party/lua/src
        ${CMAKE_BINARY_DIR}/third_party/lua

)
include_directories(${INCLUDES})

# Source files
set(SOURCES
	Volren.cpp
	LuaEntry.cpp
)
add_library(Volren ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(Volren hiveCore)
set_target_properties(Volren PROPERTIES PREFIX "")
 
