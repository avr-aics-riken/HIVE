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
	RefPtr<BufferMeshData> mesh;
protected:
	bool Load(const char* filename, bool swap_endian = false);
	BufferMeshData *MeshData();
};

#endif //_STLLOADER_H_

