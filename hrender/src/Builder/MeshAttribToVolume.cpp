/**
 * @file MeshAttribToVolume.cpp   
 * Mesh attrib データからVolumeDataへ変換するモジュール
 */
#include "MeshAttribToVolume.h"
#include "../Core/vxmath.h"
#include <string.h>
#include <algorithm>
#include <string>

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
bool MeshAttribToVolume::ToVolume(int w, int h, int d) {
    delete m_volume;

    m_volume = new BufferVolumeData();

    int c = 1; // Store scalar density.
    m_volume->Create(w, h, d, c);

    FloatBuffer* buffer = m_volume->Buffer();

    float bmin[3];
    float bmax[3];

    assert(m_point);
    assert(m_point->Position());
    assert(m_point->Position()->GetNum() > 0);

    float* voxels = m_volume->Buffer()->GetBuffer();

    const size_t fnum = w * h * d * c;
    memset(voxels, 0, fnum * sizeof(float));

    std::vector<int> countBuf(w * h * d, 0); // for compute avarage.

    float *position = m_mesh->Position()->GetBuffer();
    float *attrib   = m_attrib->Float()->GetBuffer();

    size_t dim[3] = { w, h, d };
    for (size_t i = 1; i < m_mesh->Position()->GetNum(); i++) {
        // Assume given point coordinate are all within the range of bounding box.
        size_t loc = findLoc(position[3*i+0], position[3*i+1], position[3*i+2], m_bmin, m_bmax, dim);
        voxels[loc] += attrib[i];
        countBuf[loc]++;
    }

    // Take an avarage.
    for (size_t i = 0; i < size_t(w) * size_t(h) * size_t(d); i++) {
        voxels[i] /= (float)countBuf[i];
    }

    printf("ToVolume: %zu, %zu, %zu\n", dim[0], dim[1], dim[2]);
    
    return true;
}

/**
 * 点データ設定
 * @param point 変換対象の position データ
 * @param attrib 変換対象の Attrib データ
 * @retval 1 変換成功
 * @retval 0 変換失敗
 */
int MeshAttribToVolume::Create(BufferMeshData *mesh, BufferExtraData* attrib, float bmin[3], float bmax[3]) {
    if (!mesh) {
        return 0;
    }

    if (mesh->Position()->GetNum() < 1) {
        return 0;
    }

    // @todo { Support vector type. }
    {
        std::string type = std::string(attrib->GetDataType());
        if (type != "float") {
            return 0;
        }
    }

    if (attrib->Float()->GetNum() < 1) {
        return 0;
    }

    m_mesh = mesh;
    m_attrib = attrib;

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
BufferVolumeData* MeshAttribToVolume::VolumeData()
{
    return m_volume;
}

/// コンストラクタ
MeshAttribToVolume::MeshAttribToVolume() : m_volume(0), m_mesh(0) {
}

/// デストラクタ
MeshAttribToVolume::~MeshAttribToVolume() {
}

