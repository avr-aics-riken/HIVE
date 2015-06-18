/**
 * @file SparseVolumeToVolume.cpp  
 * Create VolumeData from SparseVolumeData.
 */
#include "SparseVolumeToVolume.h"
#include "../Core/vxmath.h"
#include <string.h>


/// コンストラクタ
SparseVolumeToVolume::SparseVolumeToVolume(){
    m_volume      = new BufferVolumeData();
}


/**
 * Create VolumeData from SparseVolumeData
 * @param resamplingRate resampling rate(e.g. 0.5 -> half resolution).
 * @retval 0 Volumeが指定されていない
 * @retval >0 OK
 */
int SparseVolumeToVolume::Create(BufferSparseVolumeData *sparseVolume, float resamplingRate) {
    if (!sparseVolume) {
        return 0;
    }

	int width  = sparseVolume->Width();
	int height = sparseVolume->Height();
	int depth  = sparseVolume->Depth();
	size_t components = sparseVolume->Component();

	size_t destWidth  = width * resamplingRate;
	size_t destHeight = height * resamplingRate;
	size_t destDepth  = depth * resamplingRate;

	double invDestWidth  = 1.0 / (double)(destWidth);
	double invDestHeight = 1.0 / (double)(destHeight);
	double invDestDepth  = 1.0 / (double)(destDepth);

	printf("[SparseVolumeToVolume] Create volume with resolution: %d x %d x %d\n", (int)destWidth, (int)destHeight, (int)destDepth);

	m_volume->Create(destWidth, destHeight, destDepth, components);

	// Ensure building spatial data structure for sparse volume dataset.
	if (!sparseVolume->IsBuilt()) {
		sparseVolume->Build();
	}

	// Resample sparse voxel and create voxel data.
	for (size_t z = 0; z < destDepth; z++) {
		for (size_t y = 0; y < destHeight; y++) {
			for (size_t x = 0; x < destWidth; x++) {

				// [0.0, 1.0]
				float sx = x * invDestWidth;
				float sy = y * invDestHeight;
				float sz = z * invDestDepth;

				size_t dstIdx = z * destHeight * destWidth + y * destWidth + x;

				float ret[16]; // alloc enough resource.

				sparseVolume->Sample(ret, sx, sy, sz);

				for (int c = 0; c < components; c++) {
					m_volume->Buffer()->GetBuffer()[components * dstIdx + c] = ret[c];
				}
			}
		}
	}

	return 1; // OK
}

/**
 * 変換された Volume データへの参照を取得する
 * @return 変換された Volume データへの参照が返却される。
 */
BufferVolumeData* SparseVolumeToVolume::VolumeData()
{
    return m_volume;
}

