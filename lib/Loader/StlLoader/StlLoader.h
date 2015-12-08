/**
 * @file StlLoader.h
 * STLデータローダー
 */
#ifndef _STLLOADER_H_
#define _STLLOADER_H_

#include "Ref.h"
#include "BufferMeshData.h"

/**
 * STLデータローダー
 */
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

#endif //_STLLOADER_H_

