#ifndef _OBJLOADERNATIVE_H
#define _OBJLOADERNATIVE_H

#include "Ref.h"
#include "BufferMeshData.h"

class OBJLoader : public RefCount
{
public:
	OBJLoader();
	~OBJLoader();
	void Clear();
	BufferMeshData mesh;
protected:
	bool Load(const char* filename);
	BufferMeshData MeshData();
};

#endif //_OBJLOADERNATIVE_H