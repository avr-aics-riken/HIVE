/**
 * @file PointToVolume.cpp   
 * 点データからVolumeDataへ変換するモジュール
 */
#include "PointToVolume.h"
#include "../Core/vxmath.h"
#include <string.h>
#include <algorithm>
#include <iostream>

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
 * 点データからVolumeDataへの変換
 * @param w Width
 * @param h Height
 * @retval true 変換成功
 * @retval false 変換失敗
 */
bool PointToVolume::ToVolume(int w, int h, int d) {

    m_volume = BufferVolumeData::CreateInstance();

    int c = 1; // Store scalar density.
    m_volume->Create(w, h, d, c);

    float bmin[3];
    float bmax[3];

    assert(m_point);
    assert(m_point->Position());
    assert(m_point->Position()->GetNum() > 0);

    // Compute bounding box.
    float *position = m_point->Position()->GetBuffer();
    bmin[0] = bmax[0] = position[0];
    bmin[1] = bmax[1] = position[1];
    bmin[2] = bmax[2] = position[2];
    for (size_t i = 0; i < m_point->Position()->GetNum(); i++) {
        bmin[0] = (std::min)(bmin[0], position[3*i+0]);
        bmin[1] = (std::min)(bmin[1], position[3*i+1]);
        bmin[2] = (std::min)(bmin[2], position[3*i+2]);

        bmax[0] = (std::max)(bmax[0], position[3*i+0]);
        bmax[1] = (std::max)(bmax[1], position[3*i+1]);
        bmax[2] = (std::max)(bmax[2], position[3*i+2]);
    }
        
    printf("bmin: %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    printf("bmax: %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

    float* voxels = m_volume->Buffer()->GetBuffer();

    const size_t fnum = w * h * d * c;
    memset(voxels, 0, fnum * sizeof(float));

    // @todo { Consider particle radius. }

    size_t dim[3] = { w, h, d };
    for (size_t i = 0; i < m_point->Position()->GetNum(); i++) {
        size_t loc = findLoc(position[3*i+0], position[3*i+1], position[3*i+2], bmin, bmax, dim);
        voxels[loc] += 1.0f;
    }

    printf("ToVolume: %zu, %zu, %zu\n", dim[0], dim[1], dim[2]);
    
    return true;
}

/**
 * 点 vec3 attrib データからVolumeDataへの変換
 * @param w Width
 * @param h Height
 * @param attrib point attribute
 * @retval true 変換成功
 * @retval false 変換失敗
 */
bool PointToVolume::ToVolumeWithAttrib(int w, int h, int d, BufferExtraData *attrib) {

    assert(m_point);
    assert(m_point->Position());
    assert(m_point->Position()->GetNum() > 0);

    assert(vattrib);
    if (attrib->GetNum() != m_point->Position()->GetNum()) {
        std::cerr << "Number of elements mismatch. " << std::endl;
        return false;
    }

    std::string attribDataType = attrib->GetDataType();

    int c = 0;
    if ((attribDataType.compare("float") == 0)) {
        c = 1;
    } else if (attribDataType.compare("vec3") == 0) {
        c = 3;
    } else {
        // OK
        std::cerr << "Unsupported attribute type: " << attribDataType << std::endl;
        return false;
    }

    m_volume = BufferVolumeData::CreateInstance();

    m_volume->Create(w, h, d, c);

    float bmin[3];
    float bmax[3];

    // Compute bounding box.
    float *position = m_point->Position()->GetBuffer();
    bmin[0] = bmax[0] = position[0];
    bmin[1] = bmax[1] = position[1];
    bmin[2] = bmax[2] = position[2];
    for (size_t i = 0; i < m_point->Position()->GetNum(); i++) {
        bmin[0] = (std::min)(bmin[0], position[3*i+0]);
        bmin[1] = (std::min)(bmin[1], position[3*i+1]);
        bmin[2] = (std::min)(bmin[2], position[3*i+2]);

        bmax[0] = (std::max)(bmax[0], position[3*i+0]);
        bmax[1] = (std::max)(bmax[1], position[3*i+1]);
        bmax[2] = (std::max)(bmax[2], position[3*i+2]);
    }
        
    printf("bmin: %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    printf("bmax: %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

    float* voxels = m_volume->Buffer()->GetBuffer();

    const size_t fnum = w * h * d * c;
    memset(voxels, 0, fnum * sizeof(float));

    // @todo { Consider particle radius? }

    size_t dim[3] = { w, h, d };
    for (size_t i = 0; i < m_point->Position()->GetNum(); i++) {
        size_t loc = findLoc(position[3*i+0], position[3*i+1], position[3*i+2], bmin, bmax, dim);
        if (c == 1) {
                voxels[loc] += attrib->Float()->GetBuffer()[i];
        } else if (c == 3) {
            for (int k = 0 ; k < c; k++) {
                voxels[c * loc + k] += attrib->Vec3()->GetBuffer()[c * i + k];
            }
        }

    }

    printf("ToVolume: %zu, %zu, %zu\n", dim[0], dim[1], dim[2]);
    
    return true;
}

/**
 * 点データ設定
 * @param point 変換対象の点データ
 * @retval 1 変換成功
 * @retval 0 変換失敗
 */
int PointToVolume::Create(BufferPointData *point) {
    if (!point) {
        return 0;
    }

    if (point->Position()->GetNum() < 1) {
        return 0;
    }

    m_point = point; // just copy a pointer.

    return 1;
}

/**
 * VolumeDataへの参照取得1
 * @retval m_volume VolumeDataへの参照
 */
BufferVolumeData* PointToVolume::VolumeData()
{
    return m_volume;
}

/// コンストラクタ
PointToVolume::PointToVolume() : m_volume(0), m_point(0) {
}

/// デストラクタ
PointToVolume::~PointToVolume() {
}

