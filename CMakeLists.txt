# hrender
PROJECT(hrender)


# sub modules
SUBDIRS(third_party/lua)
SUBDIRS(third_party/SURFACE)

add_executable (hrender src/main.cpp )
