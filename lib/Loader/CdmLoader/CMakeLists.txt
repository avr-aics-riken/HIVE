IF(HIVE_BUILD_WITH_CDMLIB)

SET(HIVE_BUILD_WITH_BCMTOOLS On)
add_definitions(-DHIVE_WITH_CDMLIB)

  IF(NOT HIVE_BUILD_WITH_MPI)
    add_definitions(-D_CDM_WITHOUT_MPI_)        # Use stub MPI defines(provided in CDMlib/include/mpi_stubs.h)
  ENDIF()

  SET(CDMLIB_SOURCES
    CdmLoader.cpp
  )

  SET(CDMLIB_LIBS
    CDM TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/CDMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/CDMlib/lib)

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
	CdmLoader.cpp
	LuaEntry.cpp
)
add_library(CdmLoader SHARED ${SOURCES})
target_link_libraries(CdmLoader hiveCore)
target_link_libraries(CdmLoader ${CDMLIB_LIBS})
set_target_properties(CdmLoader PROPERTIES PREFIX "")

ENDIF() # HIVE_WITH_CDMLIB
