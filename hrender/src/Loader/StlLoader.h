#ifndef _STLLOADERNATIVE_H
#define _STLLOADERNATIVE_H

#include "Ref.h"
#include "BufferMeshData.h"

class STLLoader : public RefCount
{
public:
	STLLoader();
	~STLLoader();
	void Clear();
	BufferMeshData mesh;
protected:
	bool Load(const char* filename);
	BufferMeshData *MeshData();
};

#endif //_STLLOADERNATIVE_H
