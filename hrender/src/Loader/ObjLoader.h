#ifndef _OBJLOADERNATIVE_H
#define _OBJLOADERNATIVE_H

#include "Ref.h"

class BufferMeshData;
class BufferPointData;
class SimpleObj;

class OBJLoader : public RefCount
{
private:
    RefPtr<BufferMeshData> m_mesh;
    RefPtr<BufferPointData> m_point;
    
    BufferMeshData*  createMeshData(const SimpleObj& obj) const;
    BufferPointData* createPointData(const SimpleObj& obj) const;

public:
	OBJLoader();
	~OBJLoader();
	void Clear();
    
protected:
	bool Load(const char* filename);
    BufferMeshData* MeshData();
    BufferPointData* PointData();
};

#endif //_OBJLOADERNATIVE_H

