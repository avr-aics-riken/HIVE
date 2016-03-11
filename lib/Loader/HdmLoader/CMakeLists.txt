IF(HIVE_BUILD_WITH_HDMLIB)

SET(HIVE_BUILD_WITH_BCMTOOLS On)
add_definitions(-DHIVE_WITH_HDMLIB)

  SET(HDMLIB_LIBS
    HDM TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/Polylib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/HDMlib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/HDMlib/lib)
  
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
add_library(HdmLoader ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(HdmLoader hiveCore)
target_link_libraries(HdmLoader ${HDMLIB_LIBS})
set_target_properties(HdmLoader PROPERTIES PREFIX "")


ENDIF() # HIVE_BUILD_WITH_HDMLIB

 
