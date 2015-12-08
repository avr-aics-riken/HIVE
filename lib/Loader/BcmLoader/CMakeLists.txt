IF(HIVE_BUILD_WITH_BCMTOOLS)

add_definitions(-DHIVE_WITH_BCMTOOLS)

  SET(BCMTOOLS_SOURCES
    BcmLoader.cpp
  )

  SET(BCMTOOLS_LIBS
    BCM POLYmpi TPmpi
  )

  include_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/include)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/BCMTools/lib)
  link_directories(${CMAKE_SOURCE_DIR}/third_party/local/Polylib/lib)
  

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
	BcmLoader.cpp
	LuaEntry.cpp
)
add_library(BcmLoader SHARED ${SOURCES})
target_link_libraries(BcmLoader hiveCore)
set_target_properties(BcmLoader PROPERTIES PREFIX "")

# Additional Libraries
if (APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(HIVE_BUILD_WITH_BCMTOOLS)
        add_definitions(-U__BLOCKS__) # work around for BCMlib's Block.h header
    endif()
        find_library(COCOA Cocoa)
        if (NOT COCOA)
            message(FATAL_ERROR "Cocoa Framework not found")
        endif()
        target_link_libraries(hiveCore ${COCOA})
endif()

ENDIF() # HIVE_BUILD_WITH_BCMTOOLS
