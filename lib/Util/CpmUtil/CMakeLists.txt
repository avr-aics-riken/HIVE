IF(HIVE_BUILD_WITH_CPMLIB)

add_definitions(-DHIVE_WITH_CPMLIB)

  SET(CPMLIB_SOURCES
    CpmUtil.cpp
  )

  SET(CPMLIB_LIBS
    CPMmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/CPMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/CPMlib/lib)

# Headers
set(INCLUDES
	.
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/Core
        ${CMAKE_SOURCE_DIR}/include/SceneScript
        ${CMAKE_SOURCE_DIR}/third_party/lua/src
        ${CMAKE_BINARY_DIR}/third_party/lua

)
include_directories(${INCLUDES})

# Source files
set(SOURCES
	CpmUtil.cpp
	LuaEntry.cpp
)
add_library(CpmUtil ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(CpmUtil ${CPMLIB_LIBS})
target_link_libraries(CpmUtil hiveCore)
set_target_properties(CpmUtil PROPERTIES PREFIX "")

ENDIF() # HIVE_BUILD_WITH_CPMLIB
