/**
 * @file SolidToVolume.cpp
 * 点データからVolumeDataへ変換するモジュール
 */
#include "SolidToVolume.h"
#include "../Core/vxmath.h"
#include "vxmath.h"
#include <string.h>
#include <algorithm>

namespace {
    
    /**
     * BoundingBox[BB]中のpointデータのオフセットを求める
     * @param x X座標
     * @param y Y座標
     * @param z Z座標
     * @param bmin BB最小値
     * @param bmax ファイルパス
     * @param dim  ボリュームサイズ
     * @retval ボリュームデータ配列中のオフセット
     */
    inline size_t findLoc(float x, float y, float z, const float bmin[3], const float bmax[3], const size_t dim[3]) {
        float xx = (x - bmin[0]) / (bmax[0] - bmin[0]);
        float yy = (y - bmin[1]) / (bmax[1] - bmin[1]);
        float zz = (z - bmin[2]) / (bmax[2] - bmin[2]);
        
        size_t i = (std::max)((size_t)0, (std::min)((size_t)(dim[0]-1), (size_t)(xx*dim[0])));
        size_t j = (std::max)((size_t)0, (std::min)((size_t)(dim[1]-1), (size_t)(yy*dim[1])));
        size_t k = (std::max)((size_t)0, (std::min)((size_t)(dim[2]-1), (size_t)(zz*dim[2])));
        
        return k * dim[0] * dim[1] + j * dim[0] + i;
    }
    
}

/**
 * 多面体データからVolumeDataへの変換
 * @param w Width
 * @param h Height
 * @retval true 変換成功
 * @retval false 変換失敗
 */
bool SolidToVolume::ToVolume(int w, int h, int d) {
    
    m_volume = BufferVolumeData::CreateInstance();
    
    int c = 1; // Store scalar density.
    m_volume->Create(w, h, d, c);
    
    FloatBuffer* buffer = m_volume->Buffer();
    
    float bmin[3];
    float bmax[3];
    
    assert(m_solid);
    assert(m_solid->Position());
    assert(m_solid->Position()->GetNum() > 0);
    
#if 0 
    // Compute bounding box.
    float *position = m_solid->Position()->GetBuffer();
    bmin[0] = bmax[0] = position[0];
    bmin[1] = bmax[1] = position[1];
    bmin[2] = bmax[2] = position[2];
    for (size_t i = 1; i < m_solid->Position()->GetNum(); i++) {
        bmin[0] = (std::min)(bmin[0], position[3*i+0]);
        bmin[1] = (std::min)(bmin[1], position[3*i+1]);
        bmin[2] = (std::min)(bmin[2], position[3*i+2]);
        
        bmax[0] = (std::max)(bmax[0], position[3*i+0]);
        bmax[1] = (std::max)(bmax[1], position[3*i+1]);
        bmax[2] = (std::max)(bmax[2], position[3*i+2]);
    }
#else
    // Use user-supplied bbox at Create()
    bmin[0] = m_bmin[0];
    bmin[1] = m_bmin[1];
    bmin[2] = m_bmin[2];

    bmax[0] = m_bmax[0];
    bmax[1] = m_bmax[1];
    bmax[2] = m_bmax[2];
#endif
    
    printf("bmin: %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    printf("bmax: %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);
    
    float* voxels = m_volume->Buffer()->GetBuffer();
    
    const size_t fnum = w * h * d * c;
    memset(voxels, 0, fnum * sizeof(float));
    
    // @todo { Consider particle radius. }
    
    size_t dim[3] = { w, h, d };
    
    int faces[6][4];
    int face_n = 0;
    
    switch (m_solid->Type()) {
        case 5: //Pyramid
            face_n = 5;
            faces[0][0] = 0;
            faces[0][1] = 1;
            faces[0][2] = 2;
            faces[0][3] = 3;
            
            faces[1][0] = 0;
            faces[1][1] = 4;
            faces[1][2] = 1;
            
            faces[2][0] = 1;
            faces[2][1] = 4;
            faces[2][2] = 2;
            
            faces[3][0] = 2;
            faces[3][1] = 4;
            faces[3][2] = 3;
            
            faces[4][0] = 3;
            faces[4][1] = 4;
            faces[4][2] = 0;
            
            break;
        case 6: //Prism
            face_n = 5;
            faces[0][0] = 0;
            faces[0][1] = 1;
            faces[0][2] = 2;
            
            faces[1][0] = 3;
            faces[1][1] = 4;
            faces[1][2] = 5;
            
            faces[2][0] = 0;
            faces[2][1] = 3;
            faces[2][2] = 5;
            faces[2][3] = 1;
            
            faces[3][0] = 0;
            faces[3][1] = 2;
            faces[3][2] = 4;
            faces[3][3] = 3;
            
            faces[4][0] = 1;
            faces[4][1] = 5;
            faces[4][2] = 4;
            faces[4][3] = 2;
            break;
        case 8: //Hexahedron
            face_n = 6;
            faces[0][0] = 0;
            faces[0][1] = 1;
            faces[0][2] = 2;
            faces[0][3] = 3;
            
            faces[1][0] = 4;
            faces[1][1] = 5;
            faces[1][2] = 6;
            faces[1][3] = 7;
            
            faces[2][0] = 0;
            faces[2][1] = 4;
            faces[2][2] = 7;
            faces[2][3] = 1;
            
            faces[3][0] = 1;
            faces[3][1] = 7;
            faces[3][2] = 6;
            faces[3][3] = 2;
            
            faces[4][0] = 2;
            faces[4][1] = 6;
            faces[4][2] = 5;
            faces[4][3] = 3;
            
            faces[5][0] = 0;
            faces[5][1] = 3;
            faces[5][2] = 5;
            faces[5][3] = 4;
            break;
        default:
            // ???
            assert(0);
            break;
    }

    for (size_t i = 1; i < m_solid->Index()->GetNum(); i++) {
        float *vertices_f =  &(m_solid->Position()->GetBuffer()[i*m_solid->Type()]);
        float bmax_[3], bmin_[3];
        
        // @todo { scale vertex poisition according to volume cell size. }
        bmin_[0] = bmax_[0] = vertices_f[0];
        bmin_[1] = bmax_[1] = vertices_f[1];
        bmin_[2] = bmax_[2] = vertices_f[2];
        
        for (int i = 1; i < m_solid->Type(); i++) {
            
            bmin_[0] = (std::min)(bmin_[0], vertices_f[3*i+0]);
            bmin_[1] = (std::min)(bmin_[1], vertices_f[3*i+1]);
            bmin_[2] = (std::min)(bmin_[2], vertices_f[3*i+2]);
            
            bmax_[0] = (std::max)(bmax_[0], vertices_f[3*i+0]);
            bmax_[1] = (std::max)(bmax_[1], vertices_f[3*i+1]);
            bmax_[2] = (std::max)(bmax_[2], vertices_f[3*i+2]);
        }
        VX::Math::vec3 verts[12] = {};
       
        for (int i = 0; i < m_solid->Type(); i++) {
            verts[i].x = vertices_f[i*3];
            verts[i].y = vertices_f[i*3 + 1];
            verts[i].z = vertices_f[i*3 + 2];
        }
        
        for (int x = bmin_[0]; x <= bmax_[0]; x++)
            for (int y = bmin_[1]; y <= bmax_[1]; y++)
                for (int z = bmin_[2]; z <= bmax_[2]; z++){
                    float d = -1e16;
                    VX::Math::vec3 p(x, y, z);
                    
                    for(int i = 0; i < face_n; i ++)
                        d = std::max(d, dot(normalize(cross(verts[faces[i][1]] - verts[faces[i][0]],
                                                            verts[faces[i][2]] - verts[faces[i][1]])),
                                            p - verts[faces[i][0]] ) );
                    
                    if(d < 0){
                        size_t loc = findLoc(x, y, z, bmin, bmax, dim);
                        voxels[loc] += 1.0f;
                    }
                }
    }
    
    printf("ToVolume: %zu, %zu, %zu\n", dim[0], dim[1], dim[2]);
    
    return true;
}

/**
 * 多面体データ設定
 * @param solid 変換対象の多面体データ
 * @retval 1 変換成功
 * @retval 0 変換失敗
 */
int SolidToVolume::Create(BufferSolidData *solid, float bmin[3], float bmax[3]) {
    if (!solid) {
        return 0;
    }
    
    if (solid->Position()->GetNum() < 1) {
        return 0;
    }

    m_solid = solid; // just copy a pointer.

    m_bmin[0] = bmin[0];
    m_bmin[1] = bmin[1];
    m_bmin[2] = bmin[2];

    m_bmax[0] = bmax[0];
    m_bmax[1] = bmax[1];
    m_bmax[2] = bmax[2];
    
    return 1;
}

/**
 * VolumeDataへの参照取得1
 * @retval m_volume VolumeDataへの参照
 */
BufferVolumeData* SolidToVolume::VolumeData()
{
    return m_volume;
}

/// コンストラクタ
SolidToVolume::SolidToVolume() : m_volume(0), m_solid(0) {
    m_bmin[0] = 0.0f;
    m_bmin[1] = 0.0f;
    m_bmin[2] = 0.0f;
    m_bmax[0] = 0.0f;
    m_bmax[1] = 0.0f;
    m_bmax[2] = 0.0f;
}

/// デストラクタ
SolidToVolume::~SolidToVolume() {
}

