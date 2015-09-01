/**
 * @file MeshAttribToVolume.cpp   
 * Mesh attrib データからVolumeDataへ変換するモジュール
 */
#include "MeshAttribToVolume.h"
#include "../Core/vxmath.h"
#include <string.h>
#include <algorithm>
#include <string>
#include <cfloat>

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

inline size_t Clamp(size_t x, size_t x0, size_t x1)
{
	size_t i = (std::max)(x0, (std::min)(x1, x));

	return i;
}

/**
 * BoundingBox[BB]中のpointデータのオフセットを求める(nearest 27 points)
 * @param x X座標
 * @param y Y座標
 * @param z Z座標
 * @param bmin BB最小値
 * @param bmax ファイルパス
 * @param dim  ボリュームサイズ
 * @retval ボリュームデータ配列中のオフセット
 */
inline void findLoc27(size_t locs[27], float x, float y, float z, const float bmin[3], const float bmax[3], const size_t dim[3]) {
    float xx = (x - bmin[0]) / (bmax[0] - bmin[0]);
    float yy = (y - bmin[1]) / (bmax[1] - bmin[1]);
    float zz = (z - bmin[2]) / (bmax[2] - bmin[2]);

    size_t i = (std::max)((size_t)0, (std::min)((size_t)(dim[0]-1), (size_t)(xx*dim[0])));
    size_t j = (std::max)((size_t)0, (std::min)((size_t)(dim[1]-1), (size_t)(yy*dim[1])));
    size_t k = (std::max)((size_t)0, (std::min)((size_t)(dim[2]-1), (size_t)(zz*dim[2])));

	size_t i0 = Clamp(i-1, 0, dim[0]-1);
	size_t i1 = Clamp(i+1, 0, dim[0]-1);

	size_t j0 = Clamp(j-1, 0, dim[1]-1);
	size_t j1 = Clamp(j+1, 0, dim[1]-1);

	size_t k0 = Clamp(k-1, 0, dim[2]-1);
	size_t k1 = Clamp(k+1, 0, dim[2]-1);

	locs[0] = k0 * dim[0] * dim[1] + j0 * dim[0] + i0;
	locs[1] = k0 * dim[0] * dim[1] + j0 * dim[0] + i;
	locs[2] = k0 * dim[0] * dim[1] + j0 * dim[0] + i1;

	locs[3] = k0 * dim[0] * dim[1] + j * dim[0] + i0;
	locs[4] = k0 * dim[0] * dim[1] + j * dim[0] + i;
	locs[5] = k0 * dim[0] * dim[1] + j * dim[0] + i1;

	locs[6] = k0 * dim[0] * dim[1] + j1 * dim[0] + i0;
	locs[7] = k0 * dim[0] * dim[1] + j1 * dim[0] + i;
	locs[8] = k0 * dim[0] * dim[1] + j1 * dim[0] + i1;

	locs[9]  = k * dim[0] * dim[1] + j0 * dim[0] + i0;
	locs[10] = k * dim[0] * dim[1] + j0 * dim[0] + i;
	locs[11] = k * dim[0] * dim[1] + j0 * dim[0] + i1;

	locs[12] = k * dim[0] * dim[1] + j * dim[0] + i0;
	locs[13] = k * dim[0] * dim[1] + j * dim[0] + i;
	locs[14] = k * dim[0] * dim[1] + j * dim[0] + i1;

	locs[15] = k * dim[0] * dim[1] + j1 * dim[0] + i0;
	locs[16] = k * dim[0] * dim[1] + j1 * dim[0] + i;
	locs[17] = k * dim[0] * dim[1] + j1 * dim[0] + i1;

	locs[18] = k1 * dim[0] * dim[1] + j0 * dim[0] + i0;
	locs[19] = k1 * dim[0] * dim[1] + j0 * dim[0] + i;
	locs[20] = k1 * dim[0] * dim[1] + j0 * dim[0] + i1;

	locs[21] = k1 * dim[0] * dim[1] + j * dim[0] + i0;
	locs[22] = k1 * dim[0] * dim[1] + j * dim[0] + i;
	locs[23] = k1 * dim[0] * dim[1] + j * dim[0] + i1;

	locs[24] = k1 * dim[0] * dim[1] + j1 * dim[0] + i0;
	locs[25] = k1 * dim[0] * dim[1] + j1 * dim[0] + i;
	locs[26] = k1 * dim[0] * dim[1] + j1 * dim[0] + i1;

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

    assert(m_point);
    assert(m_point->Position());
    assert(m_point->Position()->GetNum() > 0);

    float* voxels = m_volume->Buffer()->GetBuffer();

    const size_t fnum = w * h * d * c;
    for (size_t i = 0; i < fnum; i++) {
    	voxels[i] = 0.0f; //-FLT_MAX; // @fixme
	}

    std::vector<int> countBuf(w * h * d, 0); // for compute avarage.

    float *position = m_mesh->Position()->GetBuffer();
    float *attrib   = m_attrib->Float()->GetBuffer();

    size_t dim[3] = { w, h, d };
    for (size_t i = 0; i < m_mesh->Position()->GetNum(); i++) {
        // Assume given point coordinate are all within the range of bounding box.
        size_t loc = findLoc(position[3*i+0], position[3*i+1], position[3*i+2], m_bmin, m_bmax, dim);
		if (countBuf[loc] == 0) {
			voxels[loc] = attrib[i];
		} else {
			voxels[loc] += attrib[i];
		}
        countBuf[loc]++;
		
		//size_t locs[27];
        //findLoc27(locs, position[3*i+0], position[3*i+1], position[3*i+2], m_bmin, m_bmax, dim);
		//
		//for (int k = 0; k < 27; k++) {
		//	voxels[locs[k]] += attrib[i];
		//	countBuf[locs[k]]++;
		//}

    }

    // Take an avarage.
    for (size_t i = 0; i < size_t(w) * size_t(h) * size_t(d); i++) {
		if (countBuf[i] > 0) {
			voxels[i] /= (float)countBuf[i];
		} else {
			//printf("empty: %d\n", i);
		}
    }

    printf("ToVolume: %zu, %zu, %zu\n", dim[0], dim[1], dim[2]);
    
    return true;
}

/**
 * Create volume from mesh attribute.
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

	printf("MeshAttribToVolume: bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
	printf("MeshAttribToVolume: bmax = %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);

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

