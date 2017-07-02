IF(HIVE_BUILD_WITH_UDMLIB)

add_definitions(-DHIVE_WITH_UDMLIB)

  SET(UDMLIB_LIBS
    UDMmpi TPmpi zoltan cgns
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/Polylib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/UDMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/UDMlib/lib)

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
	UdmLoader.cpp
	LuaEntry.cpp
)
add_library(UdmLoader ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(UdmLoader hiveCore)
target_link_libraries(UdmLoader ${UDMLIB_LIBS})
set_target_properties(UdmLoader PROPERTIES PREFIX "")

ENDIF() # HIVE_BUILD_WITH_UDMLIB

 
