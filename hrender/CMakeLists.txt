cmake_minimum_required(VERSION 2.8)

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)


# Headers
#include_directories(include)


# Links
#link_directories(lib)
#set_target_properties(hrender LINK_FLAGS -lc)


# Source files
set(SRC
	src/Core/main.cpp
)

add_executable (hrender ${SRC})
#target_link_libraries(hrender LSGLES)


# Test
add_subdirectory(test)

