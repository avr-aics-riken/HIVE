PROJECT(pdb2pdm)

# --------------------------------------
# MPI
IF(HIVE_BUILD_WITH_MPI)
  add_definitions(-DHIVE_ENABLE_MPI)
ENDIF()
# --------------------------------------

IF(NOT HIVE_BUILD_WITH_PDMLIB)
  MESSAGE(FATAL_ERROR "HIVE_BUILD_WITH_PDMLIB must be enabled to build pdb2pdm")
ENDIF()

include_directories(${PROJECT_SOURCE_DIR}/../../third_party/local/include)
include_directories(${PROJECT_SOURCE_DIR}/../../third_party/local/TextParser/include)
include_directories(${PROJECT_SOURCE_DIR}/../../third_party/local/PDMlib/include)
link_directories(${PROJECT_SOURCE_DIR}/../../third_party/local/TextParser/lib)
link_directories(${PROJECT_SOURCE_DIR}/../../third_party/local/PDMlib/lib)
link_directories(${PROJECT_SOURCE_DIR}/../../third_party/local/lib)
  
SET(SRCS
  main.cc
  tiny_pdb.cc
  )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

ADD_EXECUTABLE(pdb2pdm ${SRCS})

TARGET_LINK_LIBRARIES(pdb2pdm PDM TPmpi fpzip zoltan)

# --
INSTALL(TARGETS pdb2pdm DESTINATION bin)
# --
