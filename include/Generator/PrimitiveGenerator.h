/**
 * @file PrimitiveGenerator.h
 * プリミティブジェネレーター
 */

#ifndef _HIVE_PRIMITIVEGENERATOR_H
#define _HIVE_PRIMITIVEGENERATOR_H

#include "Ref.h"

class BufferMeshData;
class BufferPointData;
class BufferLineData;
class BufferTetraData;
class BufferVectorData;

/**
 * プリミティブジェネレーター
 */
class PrimitiveGenerator : public RefCount
{
public:
	PrimitiveGenerator();
	~PrimitiveGenerator();
  
protected:
	bool Load(const char* filename);
    BufferMeshData*  Quad(float width, float height) const;
    BufferPointData* Sphere(float radius) const;
    BufferMeshData*  Cube(float width, float height, float depth) const;
    BufferMeshData*  Teapot(float scale) const;
    // Create vector arrow list
    BufferVectorData* VectorArrowList(const float *positions, const float *directions,  float num) const;
    BufferPointData*  SphereList(float *p, float num, float radius) const;
    BufferLineData*  LineList(float *vertices, float num, float radius) const;
    BufferMeshData*  TriangleList(float *vertices, float num) const;
};

#endif //_HIVE_PRIMITIVEGENERATOR_H
