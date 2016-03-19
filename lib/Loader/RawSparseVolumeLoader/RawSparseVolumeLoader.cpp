/**
 * @file RawSparseVolumeLoader.cpp
 * Raw Volume データローダー
 */
#include <stdio.h>
#include <string.h>
#include "RawSparseVolumeLoader.h"
#include "BufferVolumeData.h"

#include "InSitu/HIVESparseVolume.h"

/// コンストラクタ
RawSparseVolumeLoader::RawSparseVolumeLoader()
{
    m_sparseVolume = BufferSparseVolumeData::CreateInstance();
    Clear();
}
/// デストラクタ
RawSparseVolumeLoader::~RawSparseVolumeLoader()
{
    Clear();
}

/// メンバクリア
void RawSparseVolumeLoader::Clear()
{
    m_sparseVolume->Clear();
}

#if 0
/**
 * Raw Volume データのロード
 * @param filename ファイルパス
 * @param width      width
 * @param height     height
 * @param depth      depth
 * @param components the number of components in the voxel
 * @param format     pixel format of the voxel("float", "uint8", ...)
 * @retval true 成功
 * @retval false 失敗
 */
bool RawSparseVolumeLoader::Load(const char* filename, int width, int height, int depth, int components, const char* format)
{
    Clear();

	// @todo { endian }
	// @todo { support various format. }

	size_t pixelDataSize = 0;
	if (strcmp(format, "float") == 0) {
		pixelDataSize = sizeof(float);
	} else {
		fprintf(stderr, "[RawSparseVolumeLoader] Unknown format: %s\n", format);
		return false;
	}

	FILE* fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr,"[RawSparseVolumeLoader] Failed to load raw volume: %s\n", filename);
        return false;
    }

	size_t len = ((size_t)width) * ((size_t)height) * ((size_t)depth) * ((size_t)components) * pixelDataSize;
    
    m_volume->Create(width, height, depth, components);
    size_t n = fread(m_volume->Buffer()->GetBuffer(), 1, len, fp);
	fclose(fp);
	if (n != len) {
        fprintf(stderr,"[RawSparseVolumeLoader] Failed to load raw volume: %llu bytes expected, but %llu bytes read.\n", (unsigned long long)len, (unsigned long long)n);
	}

    return true;
}
#endif

/**
 * Load Raw Sparse Volume from memory(pointer)
 * @param pointer The pointer to Raw Sparse Volume struct(HIVESparseVolume)
 * @retval true Sucess
 * @retval false Fail
 */
bool RawSparseVolumeLoader::LoadFromPointer(void *ptr)
{
    if (ptr == NULL) {
        printf("[RawSparseVolumeLoader] WARN: NULL input for LoadFromPointer\n");
        return false;
    }
    
    HIVESparseVolume* sparseVolume = reinterpret_cast<HIVESparseVolume*>(ptr);

    printf("sparseVolume.numBlocks = %d\n", sparseVolume->numBlocks);

    for (int i = 0; i < sparseVolume->numBlocks; i++) {
#if 0 // DBG
        printf("sparseVolume.blocks[%d].extent = %d, %d, %d\n", i,
            sparseVolume->blocks[i].extent[0],
            sparseVolume->blocks[i].extent[1],
            sparseVolume->blocks[i].extent[2]);
        printf("sparseVolume.blocks[%d].size = %d, %d, %d\n", i,
            sparseVolume->blocks[i].size[0],
            sparseVolume->blocks[i].size[1],
            sparseVolume->blocks[i].size[2]);
        printf("sparseVolume.blocks[%d].offset = %d, %d, %d\n", i,
            sparseVolume->blocks[i].offset[0],
            sparseVolume->blocks[i].offset[1],
            sparseVolume->blocks[i].offset[2]);
        printf("sparseVolume.blocks[%d].level = %d\n", i,
            sparseVolume->blocks[i].level);
        printf("sparseVolume.blocks[%d].data = %p\n", i,
            sparseVolume->blocks[i].data);
#endif
    }

    bool isDouble = false; 
    if (sparseVolume->format == HIVE_VOLUME_FORMAT_FLOAT) {
        isDouble = false;
    } else if (sparseVolume->format == HIVE_VOLUME_FORMAT_DOUBLE) {
        isDouble = true;
    } else {
        fprintf(stderr, "[RawSparseVolumeLoader] Unknown format.\n");
        return false;
    }

    int globalDim[3] = {-1, -1, -1};

    m_sparseVolume->Create();

    for (int i = 0; i < sparseVolume->numBlocks; i++) {
        HIVEVolumeBlock &vb = sparseVolume->blocks[i];

        BufferSparseVolumeData::VolumeBlockFormat format = BufferSparseVolumeData::FORMAT_FLOAT;
        if (isDouble) {
            format = BufferSparseVolumeData::FORMAT_DOUBLE;
        }

        // Just pass pointer of raw volume data.
        m_sparseVolume->AddRAWVolumeBlock(vb.level, vb.offset[0], vb.offset[1], vb.offset[2],
            vb.extent[0], vb.extent[1], vb.extent[2], vb.size[0], vb.size[1], vb.size[2], sparseVolume->components, format, vb.data);

        // Ignore sparseVolume->globalDim and compute globalDim from volume blocks.
        globalDim[0] = std::max(vb.offset[0] + vb.extent[0], globalDim[0]);
        globalDim[1] = std::max(vb.offset[1] + vb.extent[1], globalDim[1]);
        globalDim[2] = std::max(vb.offset[2] + vb.extent[2], globalDim[2]);
    }

    return true;
}

/**
 * SparseVolumeData参照取得
 * @retval BufferSparseVolumeData* SparseVolumeData参照
 */
BufferSparseVolumeData *RawSparseVolumeLoader::SparseVolumeData()
{
    return m_sparseVolume;
}

