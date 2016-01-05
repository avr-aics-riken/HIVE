# --------------------------------------
# MPI
IF(HIVE_BUILD_WITH_NANOMSG)
  add_definitions(-DHIVE_ENABLE_NANOMSG)
ENDIF()
# --------------------------------------


# Headers
set(INCLUDES
	.
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/Core
	${CMAKE_SOURCE_DIR}/include/Buffer
	${CMAKE_SOURCE_DIR}/include/SceneScript
	${CMAKE_SOURCE_DIR}/third_party/local/nanomsg/include	
	${CMAKE_SOURCE_DIR}/third_party/lua/src
	${CMAKE_BINARY_DIR}/third_party/lua
)
include_directories(${INCLUDES})

IF(HIVE_BUILD_WITH_NANOMSG)
set(LIBS
	${CMAKE_SOURCE_DIR}/third_party/local/nanomsg/lib
	)
link_directories(${LIBS})
ENDIF()


# Source files
set(SOURCES
	Connection.cpp
	MetaBinary.cpp
	easywsclient.cpp
	happyhttp.cpp
	LuaEntry.cpp
)
add_library(Network ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(Network hiveCore)

IF(HIVE_BUILD_WITH_NANOMSG)
target_link_libraries(Network nanomsg)
ENDIF()

set_target_properties(Network PROPERTIES PREFIX "")

 
