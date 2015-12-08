IF(HIVE_BUILD_WITH_UDMLIB)

add_definitions(-DHIVE_WITH_UDMLIB)

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
add_library(UdmLoader SHARED ${SOURCES})
target_link_libraries(UdmLoader hiveCore)
set_target_properties(UdmLoader PROPERTIES PREFIX "")

  SET(UDMLIB_SOURCES
    UdmLoader.cpp
  )

  SET(UDMLIB_LIBS
    udm TPmpi zoltan cgns
  )

  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/Polylib/include)
  include_directories(${PROJECT_SOURCE_DIR}/../third_party/local/UDMlib/include)
  link_directories(${PROJECT_SOURCE_DIR}/../third_party/local/UDMlib/lib)

ENDIF() # HIVE_BUILD_WITH_UDMLIB

 
