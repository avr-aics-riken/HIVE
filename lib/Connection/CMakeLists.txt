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
	Connection.cpp
	MetaBinary.cpp
	easywsclient.cpp
	happyhttp.cpp
	LuaEntry.cpp
)
add_library(Connection SHARED ${SOURCES})
target_link_libraries(Connection hiveCore)
set_target_properties(Connection PROPERTIES PREFIX "")

 
