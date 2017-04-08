/**
 * @file StlLoader.cpp
 * STLデータローダー
 */
#include <math.h>

#include "StlLoader.h"
#include "SimpleSTLB.h"
#include "SimpleSTLA.h"
#include "Buffer.h"


namespace {
    
    bool isSTLBinaryFile(const char* filename, int maxlen = 400)
    {
        FILE* fp = fopen(filename, "rb");
        if (!fp) {
            return false;
        }
        unsigned char* buf = new unsigned char[maxlen];
        memset(buf, 0, maxlen);
        fread(buf,maxlen,1,fp);
        fclose(fp);
        
        const char* stlascii_header = "solid";
        
        //space char escape
        const char* data = reinterpret_cast<const char*>(buf) ;
        int i = skipSpace(data);
        
        if (strncmp( &data[i], stlascii_header, strlen(stlascii_header)) == 0) {
            const int testlen = maxlen/4; // test 1/4 size.
            for (int i = 0; i < testlen; ++i) {
                if (buf[i] >= 0x7F || buf[i] == 0x00)
                    return true;
            }
            return false; // Ascii
        }
        return true;
    }
}


/// コンストラクタ
STLLoader::STLLoader()
{
    mesh = BufferMeshData::CreateInstance();
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
 * @param swap_endian Swap endian for Binary STL?
 * @retval true 成功
 * @retval false 失敗
 */
bool STLLoader::Load(const char* filename, bool swap_endian){
	Clear();
    
    bool r = false;
    SimpleSTL* obj;
    if (isSTLBinaryFile(filename)) {
        obj = new SimpleSTLB();
        r = obj->Load(filename, swap_endian);
    } else {
        obj = new SimpleSTLA();
        r = obj->Load(filename);
    }

    mesh->Create(obj->GetVertexNum(), obj->GetIndexNum());
    Vec3Buffer* pos = mesh->Position();
    Vec3Buffer* normal = mesh->Normal();
    FloatBuffer* mat = mesh->Material();
    UintBuffer* index = mesh->Index();
    //Vec2Buffer* texcoord = mesh->Texcoord();

	memcpy(pos->GetBuffer(), obj->GetPositionBuffer(), sizeof(float) * 3 * pos->GetNum());
	normal->Create(obj->GetVertexNum());
	memcpy(normal->GetBuffer(), obj->GetNormalBuffer(), sizeof(float) * 3 * normal->GetNum());
	mat->Create(obj->GetVertexNum());
	memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
	index->Create(obj->GetIndexNum());
	memcpy(index->GetBuffer(), obj->GetIndex(), sizeof(unsigned int) * index->GetNum());

    delete obj;
    
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

