IF(HIVE_BUILD_WITH_CDMLIB)

SET(HIVE_BUILD_WITH_BCMTOOLS On)
add_definitions(-DHIVE_WITH_CDMLIB)

  IF(NOT HIVE_BUILD_WITH_MPI)
    add_definitions(-D_CDM_WITHOUT_MPI_)        # Use stub MPI defines(provided in CDMlib/include/mpi_stubs.h)
  ENDIF()

  SET(CDMLIB_SOURCES
    CdmLoader.cpp
  )

  IF (HIVE_BUILD_K_CROSS_COMPILE) # K specific option.
    SET(NETCDF_LIBS netcdf hdf5_hl hdf5 sz)
  ELSE()
    SET(NETCDF_LIBS netcdf hdf5_hl hdf5)
  ENDIF()

  SET(CDMLIB_LIBS
    CDMmpi TPmpi CPMmpi ${NETCDF_LIBS}
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/CDMlib/include)
  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/CPMlib/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/CDMlib/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/CPMlib/lib)
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
add_library(CdmLoader ${HIVE_LIBRARY_TYPE} ${SOURCES})
target_link_libraries(CdmLoader ${CDMLIB_LIBS})
target_link_libraries(CdmLoader hiveCore)
set_target_properties(CdmLoader PROPERTIES PREFIX "")

IF (HIVE_BUILD_K_CROSS_COMPILE) # K specific option.
  set(CMAKE_SHARED_LINKER_FLAGS -L/opt/aics/netcdf/k/lib-static)
  include_directories(/opt/aics/netcdf/k/include)
  link_directories(/opt/aics/netcdf/k/lib-static)
ENDIF()

ENDIF() # HIVE_WITH_CDMLIB
