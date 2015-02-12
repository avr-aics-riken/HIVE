cmake_minimum_required(VERSION 2.8)

enable_testing()

PROJECT(HIVE)

# PROJECT OUTPUT SETTING
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# modules
add_subdirectory(third_party/SURFACE)
add_subdirectory(third_party/lua)

# build
add_subdirectory(hrender)
add_subdirectory(HIVE)
