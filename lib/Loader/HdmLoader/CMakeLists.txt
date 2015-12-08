
IF(HIVE_BUILD_WITH_HDMLIB)
  add_definitions(-DHIVE_WITH_HDMLIB)
ENDIF()

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
	HdmLoader.cpp
	LuaEntry.cpp
)
add_library(HdmLoader SHARED ${SOURCES})
target_link_libraries(HdmLoader hiveCore)
set_target_properties(HdmLoader PROPERTIES PREFIX "")


IF(HIVE_BUILD_WITH_HDMLIB)

  SET(HDMLIB_SOURCES
    HdmLoader.cpp
  )

  SET(HDMLIB_LIBS
    HDM TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/Polylib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/HDMlib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/HDMlib/lib)
  
ENDIF()


 
