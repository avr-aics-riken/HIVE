#ifndef _OBJLOADERNATIVE_H
#define _OBJLOADERNATIVE_H

#include "Ref.h"

class BufferMeshData;
class BufferPointData;
class BufferLineData;
class SimpleObj;

class OBJLoader : public RefCount
{
private:
    RefPtr<BufferMeshData> m_mesh;
    RefPtr<BufferPointData> m_point;
    RefPtr<BufferLineData> m_line;
    
    SimpleObj*  m_obj;
    
    BufferMeshData*  createMeshData(const SimpleObj* obj)  const;
    BufferPointData* createPointData(const SimpleObj* obj) const;
    BufferLineData* createLineData(const SimpleObj* obj)   const;

public:
	OBJLoader();
	~OBJLoader();
	void Clear();
    
protected:
	bool Load(const char* filename);
    BufferMeshData*  MeshData();
    BufferPointData* PointData();
    BufferLineData*  LineData();
};

#endif //_OBJLOADERNATIVE_H

