# Buffers in HIVE.

## BufferData

`BufferData` is the general data buffer in HIVE, and all renderable primitives and image buffer are derived from this class.

### BufferImageData

Represents buffer for image data.

### BufferMeshData

`BufferMeshData` represents polygon(triangle) primitive
`BufferMeshData` could have face indices, material ids and normals.
Optionally, `BufferMeshData` could have `BufferExtraData`, which holds arbitrary scalar/vector variables for each vertex in `BufferMeshData`(similar to glVertexAttributes).

### BufferPointData

`BufferPointData` represents point primitive.
`BufferPointData` could have indices, material ids and per-point radius.
`BufferPointData` is rendererd as a sphere with a radius in SURFACE.

### BufferLineData

`BufferLineData` represents line primitive.
`BufferLineData` could have indices, material ids and per-vertex radius.
`BufferLineData` is rendererd as a cylinder in SURFACE.

### BufferVectorData

`BufferVectorData` represents vector arrow primitive. Its composed of a tetra primitive and a line primitive.

### BufferVolumeData

`BufferVolumeData` represents uniform and non-uniform cartesian volume primitive(simiar to 3D texture).

### BufferSparseVolumeData

`BufferSparseVolumeData` represents sparse and multiresolution volume primitiv(simiar to 3D sparse texture).

### BufferTetraData

Represents the list of tetra primitives. Tetra primitive is composed of 4 vertices(`TETRA_4` in CGNS).

For tetra primitive, the definition of vertex ordering is followed by the definition in CGNS http://cgns.github.io/CGNS_docs_current/sids/conv.html.

### BufferSolidData

For solid primitive, the definition of vertex ordering is followed by the definition in CGNS http://cgns.github.io/CGNS_docs_current/sids/conv.html.

#### Pyramid

BufferSolidData with type 5 reprensents Pyramid primitive and its composed of 5 vertices(`PYRA_5` in CGNS).

Planes composed by Pyramid pritimive must be all coplanar.

#### Prism

BufferSolidData with type 6 reprensents Prism primitive and its composed of 6 vertives(`PENTA_6` in CGNS)

Planes composed by Prism pritimive must be all coplanar.

#### Hexa

BufferSolidData with type 8 reprensents Hexahedron primitive and its composed of 8 vertives(`HEXA_8` in CGNS)

Planes composed by Hexa pritimive must be all coplanar.

