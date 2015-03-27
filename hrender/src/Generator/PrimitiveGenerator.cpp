/**
 * @file PrimitiveGenerator.cpp
 * プリミティブジェネレーター
 */
#include "PrimitiveGenerator.h"
#include "BufferMeshData.h"
#include "BufferPointData.h"
#include "BufferLineData.h"
#include "BufferTetraData.h"
#include "BufferVectorData.h"
#include "Buffer.h"
#include <string.h>

/// コンストラクタ
PrimitiveGenerator::PrimitiveGenerator()
{
}

/// デストラクタ
PrimitiveGenerator::~PrimitiveGenerator()
{
}


/**
 * 矩形メッシュデータ作成
 * @param width  幅
 * @param height 高さ
 * @retval BufferMeshData メッシュデータ
 */
BufferMeshData* PrimitiveGenerator::Quad(float width, float height) const
{
    BufferMeshData* mesh  = new BufferMeshData();
    mesh->Create(6, 6); // facevarying triangles.
    Vec3Buffer* pos      = mesh->Position();
    Vec3Buffer* normal   = mesh->Normal();
    FloatBuffer* mat     = mesh->Material();
    UintBuffer* index    = mesh->Index();
    Vec2Buffer* texcoord = mesh->Texcoord();

    float vertices[] = {
        -1.0f*width, -1.0f*height, 0.0f,
         1.0f*width, -1.0f*height, 0.0f,
        -1.0f*width,  1.0f*height, 0.0f,

        -1.0f*width,  1.0f*height, 0.0f,
         1.0f*width, -1.0f*height, 0.0f,
         1.0f*width,  1.0f*height, 0.0f
    };

    float normals[] = {
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,

         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f
    };

    float uvs[] = {
         0.0f,  0.0f,
         1.0f,  0.0f,
         0.0f,  1.0f,

         0.0f,  1.0f,
         1.0f,  0.0f,
         1.0f,  1.0f
    };

    int indices[] = {0, 1, 2, 3, 4, 5};
    
    int numVerts = 6;
    int numIndices = 6;

    pos->Create(numVerts);
    float* pp = pos->GetBuffer();
    memcpy(pp, vertices, sizeof(float)*3*numVerts);
    normal->Create(numVerts);
    memcpy(normal->GetBuffer(), normals, sizeof(float)*3*numVerts);
    texcoord->Create(numVerts);
    float* uv = texcoord->GetBuffer();
    memcpy(uv, uvs, sizeof(float)*2*numVerts);
    mat->Create(numVerts);
    memset(mat->GetBuffer(), 0, sizeof(float) * numVerts);
    index->Create(numIndices);
    memcpy(index->GetBuffer(), indices, sizeof(unsigned int) * numIndices);

    return mesh;
}


/**
 * 球メッシュデータ作成
 * @param radius 半径
 * @retval BufferMeshData メッシュデータ
 */
BufferPointData* PrimitiveGenerator::Sphere(float radius) const
{
    BufferPointData* point = new BufferPointData();

    point->Create(1);
    Vec3Buffer*  pos     = point->Position();
    FloatBuffer* mat     = point->Material();
    FloatBuffer* rad     = point->Radius();

    float vertices[] = {0.0f, 0.0f, 0.0f};

    float* pp = pos->GetBuffer();
    memcpy(pp, vertices, sizeof(float)*3);

    float* rp = rad->GetBuffer();
    rp[0] = radius;

    memset(mat->GetBuffer(), 0, sizeof(float));

    return point;
}


/**
 * 立方体メッシュデータ作成
 * @param width  幅
 * @param height 高さ
 * @retval BufferMeshData メッシュデータ
 */
BufferMeshData* PrimitiveGenerator::Cube(float width, float height, float depth) const
{
    BufferMeshData* mesh  = new BufferMeshData();
    mesh->Create(6, 6); // facevarying triangles.
    Vec3Buffer* pos      = mesh->Position();
    Vec3Buffer* normal   = mesh->Normal();
    FloatBuffer* mat     = mesh->Material();
    UintBuffer* index    = mesh->Index();
    Vec2Buffer* texcoord = mesh->Texcoord();
    
    const float vertices[][3] = {
        // +Z
        {-1.0*width, -1.0*height,  1.0*depth},
        { 1.0*width, -1.0*height,  1.0*depth},
        { 1.0*width,  1.0*height,  1.0*depth},
        {-1.0*width,  1.0*height,  1.0*depth},
        // -Z
        {-1.0*width, -1.0*height, -1.0*depth},
        {-1.0*width,  1.0*height, -1.0*depth},
        { 1.0*width,  1.0*height, -1.0*depth},
        { 1.0*width, -1.0*height, -1.0*depth},

        // +Y
        {-1.0*width,  1.0*height, -1.0*depth},
        {-1.0*width,  1.0*height,  1.0*depth},
        { 1.0*width,  1.0*height,  1.0*depth},
        { 1.0*width,  1.0*height, -1.0*depth},

        // -Y
        {-1.0*width, -1.0*height, -1.0*depth},
        { 1.0*width, -1.0*height, -1.0*depth},
        { 1.0*width, -1.0*height,  1.0*depth},
        {-1.0*width, -1.0*height,  1.0*depth},

        // +X
        { 1.0*width, -1.0*height, -1.0*depth},
        { 1.0*width,  1.0*height, -1.0*depth},
        { 1.0*width,  1.0*height,  1.0*depth},
        { 1.0*width, -1.0*height,  1.0*depth},

        // -X
        {-1.0*width, -1.0*height, -1.0*depth},
        {-1.0*width, -1.0*height,  1.0*depth},
        {-1.0*width,  1.0*height,  1.0*depth},
        {-1.0*width,  1.0*height, -1.0*depth}
    };
    
    const float normals[][3] = {
        // +Z
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        {0.0f,  0.0f,  1.0f},
        // -Z
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        {0.0f,  0.0f, -1.0f},
        // +Y
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        {0.0f,  1.0f,  0.0f},
        // -Y
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        {0.0f, -1.0f,  0.0f},
        // +X
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        {1.0f,  0.0f,  0.0f},
        // -X
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f}
    };
    
    const float uvs[][2] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},

        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},
        {1.0, 0.0},
        
        {0.0, 1.0},
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
        
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
        
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0},
        {1.0, 0.0}
    };
    
    const int indices[] = {
         0,  1,  2,  0,  2,  3,  // +Z
         4,  5,  6,  4,  6,  7,  // -Z
         8,  9, 10,  8, 10, 11,  // +Y
        12, 13, 14, 12, 14, 15,  // -Y
        16, 17, 18, 16, 18, 19,  // +X
        20, 21, 22, 20, 22, 23   // -X
    };
    const int numVerts = 24;
    const int numIndices = 36;
    
    pos->Create(numVerts);
    float* pp = pos->GetBuffer();
    memcpy(pp, vertices, sizeof(float)*3*numVerts);
    normal->Create(numVerts);
    memcpy(normal->GetBuffer(), normals, sizeof(float)*3*numVerts);
    texcoord->Create(numVerts);
    float* uv = texcoord->GetBuffer();
    memcpy(uv, uvs, sizeof(float)*2*numVerts);
    mat->Create(numVerts);
    memset(mat->GetBuffer(), 0, sizeof(float) * numVerts);
    index->Create(numIndices);
    memcpy(index->GetBuffer(), indices, sizeof(unsigned int) * numIndices);
    
    return mesh;
}

BufferMeshData* PrimitiveGenerator::Teapot(float scale) const
{
    return 0;
}
