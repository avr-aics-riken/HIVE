cmake_minimum_required(VERSION 2.8)

enable_testing()

PROJECT(HIVE)

# modules
add_subdirectory(third_party/SURFACE)
add_subdirectory(third_party/lua)

# build
add_subdirectory(hrender)
add_subdirectory(HIVE)
