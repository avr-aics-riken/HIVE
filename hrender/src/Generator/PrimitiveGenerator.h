#ifndef _HIVE_PRIMITIVEGENERATOR_H
#define _HIVE_PRIMITIVEGENERATOR_H

#include "Ref.h"

class BufferMeshData;
class BufferPointData;
class BufferLineData;
class BufferTetraData;
class BufferVectorData;

class PrimitiveGenerator : public RefCount
{
public:
	PrimitiveGenerator();
	~PrimitiveGenerator();
  
protected:
	bool Load(const char* filename);
  BufferMeshData*   Quad(float width, float height) const;
  //BufferMeshData*   Cube() const; // @todo
  BufferPointData*  Sphere(float radius) const;
  //BufferLineData*   Line() const; // @todo
  //BufferVectorData* VectorArrow() const; // @todo

};

#endif //_HIVE_PRIMITIVEGENERATOR_H
