#include <math.h>

#include "StlLoader.h"
#include "SimpleSTLB.h"

STLLoader::STLLoader()
{
	mesh.Clear();
}

STLLoader::~STLLoader(){};

void STLLoader::Clear()
{
	mesh.Clear();
}

bool STLLoader::Load(const char* filename){
	Clear();
	SimpleSTLB obj;
	bool r = obj.Load(filename);
    
	Vec3Buffer* pos = new Vec3Buffer();
	Vec3Buffer* normal = new Vec3Buffer();
	FloatBuffer* mat = new FloatBuffer();
	UintBuffer* index = new UintBuffer();
	Vec2Buffer* texcoord = new Vec2Buffer();
    
	pos->Create(obj.GetVertexNum());
	memcpy(pos->GetBuffer(), obj.GetPositionBuffer(), sizeof(float) * 3 * pos->GetNum());
	normal->Create(obj.GetVertexNum());
	memcpy(normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float) * 3 * normal->GetNum());
	mat->Create(obj.GetVertexNum());
	memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
	index->Create(obj.GetIndexNum());
	memcpy(index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int) * index->GetNum());
    mesh.Create(pos, normal, texcoord, index, mat);
    
	printf("%d\n", pos->GetNum()     );
	printf("%d\n", normal->GetNum()  );
	printf("%d\n", mat->GetNum()     );
	printf("%d\n", index->GetNum()   );
	return r;
}

BufferMeshData *STLLoader::MeshData()
{
	return &mesh;
}

