# Headers
set(INCLUDES
	.
	../
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/Core
	${CMAKE_SOURCE_DIR}/include/Buffer
	${CMAKE_SOURCE_DIR}/include/RenderObject
        ${CMAKE_SOURCE_DIR}/include/SceneScript
        ${CMAKE_SOURCE_DIR}/third_party/lua/src
        ${CMAKE_BINARY_DIR}/third_party/lua

)
include_directories(${INCLUDES})

# Source files
set(SOURCES
	PolygonAnalyzer.cpp
	VolumeAnalyzer.cpp
	SparseVolumeAnalyzer.cpp
	SolidAnalyzer.cpp
	LuaEntry.cpp
)

add_library(Analyzer ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(Analyzer hiveCore)
set_target_properties(Analyzer PROPERTIES PREFIX "")

