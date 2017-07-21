IF(HIVE_BUILD_WITH_TPUTIL)

add_definitions(-DHIVE_WITH_TPUTIL)

  SET(TPLIB_SOURCES
    TpUtil.cpp
  )

  SET(TPLIB_LIBS
    TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/TextParser/lib)

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
	TpUtil.cpp
	LuaEntry.cpp
)
add_library(TpUtil ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(TpUtil ${TPLIB_LIBS})
target_link_libraries(TpUtil hiveCore)
set_target_properties(TpUtil PROPERTIES PREFIX "")

ENDIF() # HIVE_BUILD_WITH_TPUTIL
