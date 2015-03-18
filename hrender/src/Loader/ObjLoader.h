/**
 * @file ObjLoader.h
 * OBJデータローダー
 */

#ifndef _OBJLOADERNATIVE_H
#define _OBJLOADERNATIVE_H

#include "Ref.h"

class BufferMeshData;
class BufferPointData;
class BufferLineData;
class BufferTetraData;
class BufferVectorData;
class SimpleObj;

/**
 * OBJデータローダー
 */
class OBJLoader : public RefCount
{
private:
    RefPtr<BufferMeshData>   m_mesh;
    RefPtr<BufferPointData>  m_point;
    RefPtr<BufferLineData>   m_line;
    RefPtr<BufferTetraData>  m_tetra;
    RefPtr<BufferVectorData> m_normal;
    
    SimpleObj*  m_obj;
    
    BufferMeshData*   createMeshData (const SimpleObj* obj) const;
    BufferPointData*  createPointData(const SimpleObj* obj) const;
    BufferLineData*   createLineData (const SimpleObj* obj) const;
    BufferTetraData*  createTetraData(const SimpleObj* obj) const;
    BufferVectorData* createNormalVectorData(const SimpleObj* obj) const;

public:
	OBJLoader();
	~OBJLoader();
	void Clear();
    
protected:
	bool Load(const char* filename);
    BufferMeshData*   MeshData();
    BufferPointData*  PointData();
    BufferLineData*   LineData();
    BufferTetraData*  TetraData();
    BufferVectorData* NormalData();
};

#endif //_OBJLOADERNATIVE_H

