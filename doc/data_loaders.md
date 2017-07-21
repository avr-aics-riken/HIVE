# Data loader module.

Data loader module is a module to load renderable primitives(e.g. mesh, particle, volume) into HIVE.
The code is located at `lib/Loader`.
There are some data loaders in HIVE. The user can write their own data loader using plug-in data loader interface.

The follwing is the data loader currently provided by HIVE.

## ObjLoader

Loads wavefront .obj data as a mesh primitive(polygon).

## ObjLoader

Loads wavefront .obj data as a mesh primitive(polygon).

## RawVolumeLoader

Loads simple RAW format(HIVE specific) of volume data.

## VolLoader

Loads simple VOL format(HIVE specific) of volume data.

## SphVolumeLoader

Loads SPH format of volume data.

## RawSparseVolumeLoader

Loads simple RAW format of Sparse Volume data. RawSparseVolumeLoader is used for In-Situ visualization.

## VtkLoader

Loads PImageData VTK format of volume data.

## UDMLoader

CMake option: `HIVE_BUILD_WITH_UDMLIB`

UDMLoader uses `UDMlib` to load unstructured data.

* Polygon(TRI_3, QUAD_4)
  * QUAD_4 is converted to 2 triangles.
* Tetra(TETRA_4)
  * Representad as BufferTetraData.
* Prism(PENTA_6)
  * Representad as BufferSolidData.
* Pyramid(PYRA_5)
  * Representad as BufferSolidData.
* Hexa(HEXA_8)
  * Representad as BufferSolidData.

Tetra, Prism, Pyramid and Hexa are represented as Solid primitive.

## PDMLoader

CMake option: `HIVE_BUILD_WITH_PDMLIB`

PDMLoader uses `PDMlib` to load particle data.

## CDMLoader

CMake option: `HIVE_BUILD_WITH_CDMLIB`

CDMLoader uses `CDMlib` to load cartesian volume data.
Uniform and non-uniform cartesian volume are supported.

## HDMLoader

CMake option: `HIVE_BUILD_WITH_HDMLIB`

HDMLoader uses `HDMLIB` to load hierarchical cartesian volume data.
Volume data is represented as `SparseVolume` in HIVE and SURFACE.
