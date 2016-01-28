cmake_minimum_required(VERSION 2.8)
enable_testing()

PROJECT(hrender)

# macros
#add_definitions(-DDEBUG)

# ------------------------------------------------------------

message(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})

# --------------------------------------
# 
#  SAME Build Option with SURFACE
#
# Build options
#
# K/FX10 native environment
IF (CMAKE_SYSTEM_PROCESSOR MATCHES "s64fx")

  # Use native Fujitsu compiler
  IF (HIVE_BUILD_WITH_MPI)
    SET(CMAKE_C_COMPILER mpifcc)
    SET(CMAKE_CXX_COMPILER mpiFCC)
  ELSE()
    SET(CMAKE_C_COMPILER fcc)
    SET(CMAKE_CXX_COMPILER FCC)
  ENDIF()

  # gcc compatible flag(-Xg)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xg")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xg")

  # PIC flag
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

  # OpenMP flag(-Kopenmp)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kopenmp")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kopenmp")

  IF (CMAKE_BUILD_TYPE MATCHES "Release")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast")
  ENDIF()

  # Prevent CMake from adding GNU-specific linker flags (-rdynamic)
  SET(CMAKE_C_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C cross-compiler" FORCE)
  SET(CMAKE_CXX_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C++ cross-compiler" FORCE)

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
ENDIF()

# K cross compiling
IF (HIVE_BUILD_K_CROSS_COMPILE)

  # Use Fujitsu cross compiler
  IF (HIVE_BUILD_WITH_MPI)
    SET(CMAKE_C_COMPILER mpifccpx)
    SET(CMAKE_CXX_COMPILER mpiFCCpx)
  ELSE()
    SET(CMAKE_C_COMPILER fccpx)
    SET(CMAKE_CXX_COMPILER FCCpx)
  ENDIF()

  # Prevent CMake from adding GNU-specific linker flags (-rdynamic)
  SET(CMAKE_C_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C cross-compiler" FORCE)
  SET(CMAKE_CXX_COMPILER_ID "Fujitsu" CACHE STRING "Fujitsu C++ cross-compiler" FORCE)

  # gcc compatible flag(-Xg)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xg")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xg")

  # PIC flag
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

  # OpenMP flag(-Kopenmp)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kopenmp")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kopenmp")

  IF (CMAKE_BUILD_TYPE MATCHES "Release")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast")
  ENDIF()

  # FindOpenMP.cmake does not try -Kopenmp,but the package will try specific
  # flags based on the compier ID.
  SET(OMP_FLAG_Fujitsu "-Kopenmp")
  
ELSE()
  IF (CMAKE_SYSTEM_PROCESSOR MATCHES "s64fx")
  ELSE () # assume gcc + linux or macosx
    # PIC flag
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

    IF (HIVE_BUILD_WITH_OPENMP)
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    ENDIF()
  ENDIF()
ENDIF()

# --------------------------------------
# MPI
IF(HIVE_BUILD_WITH_MPI)
  add_definitions(-DHIVE_ENABLE_MPI)
ENDIF()
# --------------------------------------

# Headers
set(INCLUDES
  ${PROJECT_SOURCE_DIR}/../third_party/lua/src
  ${CMAKE_BINARY_DIR}/third_party/lua/
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/include
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/gles
	${PROJECT_SOURCE_DIR}/../third_party/SURFACE/render
	${PROJECT_SOURCE_DIR}/../third_party/local
	${PROJECT_SOURCE_DIR}/../third_party/local/include
	${PROJECT_SOURCE_DIR}/../third_party/local/TextParser/include
	${CMAKE_SOURCE_DIR}/include
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Analyzer
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Buffer
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Builder
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Generator
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Loader
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Saver
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Network
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/RenderObject
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/SceneScript
#	${PROJECT_SOURCE_DIR}/../hiveCore/src/Core
)
INCLUDE_DIRECTORIES(${INCLUDES})


# Source files
set(HRENDER_SOURCES
  src/main.cpp
)


IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF()


# hrender
add_executable (hrender ${HRENDER_SOURCES})

# HIVE Core

link_directories(${PROJECT_SOURCE_DIR}/../bin)
target_link_libraries(hrender hiveCore)
target_link_libraries(hrender Network)


#
# For INSTALL
#
install(TARGETS hrender DESTINATION bin)

#
# for TEST
#
add_test(NAME loader_obj.scn             COMMAND hrender  ../../hrender/test/loader_obj.scn           )
add_test(NAME loader_sph.scn             COMMAND hrender  ../../hrender/test/loader_sph.scn           )
add_test(NAME loader_cdm.scn             COMMAND hrender  ../../hrender/test/loader_cdm.scn           )
add_test(NAME loader_stl.scn             COMMAND hrender  ../../hrender/test/loader_stl.scn           )
add_test(NAME loader_pdb.scn             COMMAND hrender  ../../hrender/test/loader_pdb.scn           )
add_test(NAME loader_vol.scn             COMMAND hrender  ../../hrender/test/loader_vol.scn           )
add_test(NAME volume_to_vector.scn       COMMAND hrender  ../../hrender/test/volume_to_vector.scn     )
add_test(NAME render_basic.scn           COMMAND hrender  ../../hrender/test/render_basic.scn         )
add_test(NAME render_camera.scn          COMMAND hrender  ../../hrender/test/render_camera.scn        )
add_test(NAME render_camera2.scn         COMMAND hrender  ../../hrender/test/render_camera2.scn       )
add_test(NAME render_obj.scn             COMMAND hrender  ../../hrender/test/render_obj.scn           )
add_test(NAME render_sph.scn             COMMAND hrender  ../../hrender/test/render_sph.scn           )
add_test(NAME render_cdm.scn             COMMAND hrender  ../../hrender/test/render_cdm.scn           )
add_test(NAME render_vol.scn             COMMAND hrender  ../../hrender/test/render_vol.scn           )
add_test(NAME render_stl.scn             COMMAND hrender  ../../hrender/test/render_stl.scn           )
add_test(NAME render_pdb.scn             COMMAND hrender  ../../hrender/test/render_pdb.scn           )
add_test(NAME render_pbs.scn             COMMAND hrender  ../../hrender/test/render_pbs.scn           )
add_test(NAME render_pdm.scn             COMMAND hrender  ../../hrender/test/render_pdm.scn           )
add_test(NAME render_with_gentex.scn      COMMAND hrender ../../hrender/test/render_with_gentex.scn    )
add_test(NAME render_with_gentex_F32.scn      COMMAND hrender ../../hrender/test/render_with_gentex_F32.scn    )
add_test(NAME render_vol_gentex3d.scn      COMMAND hrender ../../hrender/test/render_vol_gentex3d.scn    )
add_test(NAME render_mirror_glass.scn      COMMAND hrender ../../hrender/test/render_mirror_glass.scn    )
add_test(NAME render_obj_point.scn       COMMAND hrender  ../../hrender/test/render_obj_point.scn     )
add_test(NAME render_obj_line.scn        COMMAND hrender  ../../hrender/test/render_obj_line.scn      )
add_test(NAME render_obj_normal_vector.scn      COMMAND hrender  ../../hrender/test/render_obj_normal_vector.scn)
add_test(NAME render_obj_extra.scn       COMMAND hrender  ../../hrender/test/render_obj_extra.scn )
add_test(NAME render_sph_vector.scn      COMMAND hrender  ../../hrender/test/render_sph_vector.scn    )
add_test(NAME render_with_texture.scn    COMMAND hrender  ../../hrender/test/render_with_texture.scn    )
add_test(NAME render_cube.scn            COMMAND hrender  ../../hrender/test/render_cube.scn    )
add_test(NAME render_pointlist.scn       COMMAND hrender  ../../hrender/test/render_pointlist.scn    )
add_test(NAME render_transform_obj.scn   COMMAND hrender  ../../hrender/test/render_obj_transform.scn )
add_test(NAME render_transform_sph.scn   COMMAND hrender  ../../hrender/test/render_sph_transform.scn )
#ERROR CASE
add_test(NAME loader_obj_err.scn         COMMAND hrender ../../hrender/test/err/loader_obj_err.scn           )
add_test(NAME loader_sph_err.scn         COMMAND hrender ../../hrender/test/err/loader_sph_err.scn           )
add_test(NAME loader_stl_err.scn         COMMAND hrender ../../hrender/test/err/loader_stl_err.scn           )
add_test(NAME loader_vol_err.scn         COMMAND hrender ../../hrender/test/err/loader_vol_err.scn           )
add_test(NAME render_obj_err.scn         COMMAND hrender ../../hrender/test/err/render_obj_err.scn           )
add_test(NAME render_sph_err.scn         COMMAND hrender ../../hrender/test/err/render_sph_err.scn           )

add_test(NAME builtin_functions.scn	COMMAND hrender ../../hrender/test/builtin_functions.scn)
add_test(NAME convert_pdb_to_pdm.scn	COMMAND hrender ../../hrender/test/convert_pdb_to_pdm.scn)
add_test(NAME get_table_test.scn	COMMAND hrender ../../hrender/test/get_table_test.scn)
add_test(NAME loader_saver_image.scn	COMMAND hrender ../../hrender/test/loader_saver_image.scn)
add_test(NAME point_to_volume.scn	COMMAND hrender ../../hrender/test/point_to_volume.scn)
add_test(NAME render_cornellbox.scn	COMMAND hrender ../../hrender/test/render_cornellbox.scn)
add_test(NAME render_float2float_sph.scn	COMMAND hrender ../../hrender/test/render_float2float_sph.scn)
add_test(NAME render_hdm.scn	COMMAND hrender ../../hrender/test/render_hdm.scn)
add_test(NAME render_ibl.scn	COMMAND hrender ../../hrender/test/render_ibl.scn)
add_test(NAME render_linelist.scn	COMMAND hrender ../../hrender/test/render_linelist.scn)
add_test(NAME render_maya.scn	COMMAND hrender ../../hrender/test/render_maya.scn)
add_test(NAME render_md.scn	COMMAND hrender ../../hrender/test/render_md.scn)
add_test(NAME render_obj_pathtrace.scn	COMMAND hrender ../../hrender/test/render_obj_pathtrace.scn)
add_test(NAME render_obj_screenparallel.scn	COMMAND hrender ../../hrender/test/render_obj_screenparallel.scn)
add_test(NAME render_pbs_reflection.scn	COMMAND hrender ../../hrender/test/render_pbs_reflection.scn)
add_test(NAME render_pbs_refraction.scn	COMMAND hrender ../../hrender/test/render_pbs_refraction.scn)
add_test(NAME render_pdb_elementsymbol_as_material.scn	COMMAND hrender ../../hrender/test/render_pdb_elementsymbol_as_material.scn)
add_test(NAME render_perlin.scn	COMMAND hrender ../../hrender/test/render_perlin.scn)
add_test(NAME render_primitiveAO.scn	COMMAND hrender ../../hrender/test/render_primitiveAO.scn)
add_test(NAME render_pvti.scn	COMMAND hrender ../../hrender/test/render_pvti.scn)
add_test(NAME render_quad.scn	COMMAND hrender ../../hrender/test/render_quad.scn)
add_test(NAME render_rawvolume.scn	COMMAND hrender ../../hrender/test/render_rawvolume.scn)
add_test(NAME render_sph_contour.scn	COMMAND hrender ../../hrender/test/render_sph_contour.scn)
add_test(NAME render_sph_isosurf.scn	COMMAND hrender ../../hrender/test/render_sph_isosurf.scn)
add_test(NAME render_sphere.scn	COMMAND hrender ../../hrender/test/render_sphere.scn)
add_test(NAME render_stl_point.scn	COMMAND hrender ../../hrender/test/render_stl_point.scn)
add_test(NAME render_teapot.scn	COMMAND hrender ../../hrender/test/render_teapot.scn)
add_test(NAME render_udm_tetra.scn	COMMAND hrender ../../hrender/test/render_udm_tetra.scn)
add_test(NAME render_vol_and_polygon.scn	COMMAND hrender ../../hrender/test/render_vol_and_polygon.scn)
add_test(NAME render_vol_cyc.scn	COMMAND hrender ../../hrender/test/render_vol_cyc.scn)
add_test(NAME render_vol_cyc2.scn	COMMAND hrender ../../hrender/test/render_vol_cyc2.scn)
add_test(NAME render_volume_cut_plane.scn	COMMAND hrender ../../hrender/test/render_volume_cut_plane.scn)
add_test(NAME render_volume_cut_plane_contour.scn	COMMAND hrender ../../hrender/test/render_volume_cut_plane_contour.scn)
add_test(NAME render_with_cyc.scn	COMMAND hrender ../../hrender/test/render_with_cyc.scn)
add_test(NAME render_with_exr.scn	COMMAND hrender ../../hrender/test/render_with_exr.scn)
add_test(NAME render_with_gentex1D.scn	COMMAND hrender ../../hrender/test/render_with_gentex1D.scn)
add_test(NAME saver_sph.scn	COMMAND hrender ../../hrender/test/saver_sph.scn)
add_test(NAME render_with_gentex3d.scn	COMMAND hrender ../../hrender/test/render_with_gentex3d.scn)
add_test(NAME render_with_gentex_RGBA32.scn	COMMAND hrender ../../hrender/test/render_with_gentex_RGBA32.scn)
add_test(NAME render_with_gentex_simple.scn	COMMAND hrender ../../hrender/test/render_with_gentex_simple.scn)
add_test(NAME render_with_texture_nofiltering.scn	COMMAND hrender ../../hrender/test/render_with_texture_nofiltering.scn)
add_test(NAME send_httpimage.scn	COMMAND hrender ../../hrender/test/send_httpimage.scn)
add_test(NAME send_httpjson.scn	COMMAND hrender ../../hrender/test/send_httpjson.scn)
add_test(NAME send_renderimage.scn	COMMAND hrender ../../hrender/test/send_renderimage.scn)
add_test(NAME send_wsbinary.scn	COMMAND hrender ../../hrender/test/send_wsbinary.scn)
add_test(NAME send_wsimage.scn	COMMAND hrender ../../hrender/test/send_wsimage.scn)
add_test(NAME volume_filter_expr.scn	COMMAND hrender ../../hrender/test/volume_filter_expr.scn)
add_test(NAME volume_filter_laplacian.scn	COMMAND hrender ../../hrender/test/volume_filter_laplacian.scn)
add_test(NAME volume_to_mesh.scn	COMMAND hrender ../../hrender/test/volume_to_mesh.scn)
add_test(NAME volume_to_mesh_layer.scn	COMMAND hrender ../../hrender/test/volume_to_mesh_layer.scn)
add_test(NAME test_webgl_scene.scn	COMMAND hrender ../../hrender/test/test_webgl_scene.scn)
