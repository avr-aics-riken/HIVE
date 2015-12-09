/**
 * @file StlLoader.cpp
 * STLデータローダー
 */
#include <math.h>

#include "StlLoader.h"
#include "SimpleSTLB.h"
#include "Buffer.h"

/// コンストラクタ
STLLoader::STLLoader()
{
	mesh->Clear();
}

/// デストラクタ
STLLoader::~STLLoader(){};

/// メンバクリア
void STLLoader::Clear()
{
	mesh->Clear();
}

/**
 * STLデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool STLLoader::Load(const char* filename){
	Clear();
	SimpleSTLB obj;
	bool r = obj.Load(filename);

    mesh->Create(obj.GetVertexNum(), obj.GetIndexNum());
    Vec3Buffer* pos = mesh->Position();
    Vec3Buffer* normal = mesh->Normal();
    FloatBuffer* mat = mesh->Material();
    UintBuffer* index = mesh->Index();
    Vec2Buffer* texcoord = mesh->Texcoord();

	memcpy(pos->GetBuffer(), obj.GetPositionBuffer(), sizeof(float) * 3 * pos->GetNum());
	normal->Create(obj.GetVertexNum());
	memcpy(normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float) * 3 * normal->GetNum());
	mat->Create(obj.GetVertexNum());
	memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
	index->Create(obj.GetIndexNum());
	memcpy(index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int) * index->GetNum());

    
	printf("%d\n", pos->GetNum()     );
	printf("%d\n", normal->GetNum()  );
	printf("%d\n", mat->GetNum()     );
	printf("%d\n", index->GetNum()   );
	return r;
}

/**
 * メッシュデータ参照
 * @retval BufferMeshData* メッシュデータへの参照
 */
BufferMeshData *STLLoader::MeshData()
{
	return mesh;
}

