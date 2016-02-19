/**
 * @file RawVolumeLoader.cpp
 * Raw Volume データローダー
 */
#include <stdio.h>
#include <string.h>
#include "RawVolumeLoader.h"

/// コンストラクタ
RawVolumeLoader::RawVolumeLoader()
{
    Clear();
}
/// デストラクタ
RawVolumeLoader::~RawVolumeLoader()
{
    Clear();
}

/// メンバクリア
void RawVolumeLoader::Clear()
{
    m_volume.Clear();
}

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
bool RawVolumeLoader::Load(const char* filename, int width, int height, int depth, int components, const char* format)
{
    Clear();

	// @todo { endian }
	// @todo { support various format. }

	size_t pixelDataSize = 0;
	if (strcmp(format, "float") == 0) {
		pixelDataSize = sizeof(float);
	} else {
		fprintf(stderr, "[RawVolumeLoader] Unknown format: %s\n", format);
		return false;
	}

	FILE* fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr,"[RawVolumeLoader] Failed to load raw volume: %s\n", filename);
        return false;
    }

	size_t len = ((size_t)width) * ((size_t)height) * ((size_t)depth) * ((size_t)components) * pixelDataSize;
    
    m_volume.Create(width, height, depth, components);
    size_t n = fread(m_volume.Buffer()->GetBuffer(), 1, len, fp);
	fclose(fp);
	if (n != len) {
        fprintf(stderr,"[RawVolumeLoader] Failed to load raw volume: %llu bytes expected, but %llu bytes read.\n", (unsigned long long)len, (unsigned long long)n);
	}

    return true;
}

/**
 * Width取得
 * @retval int Width
 */
int RawVolumeLoader::Width()    {
    return m_volume.Width();
}

/**
 * Height取得
 * @retval int Height
 */
int RawVolumeLoader::Height()   {
    return m_volume.Height();
}

/**
 * Depth取得
 * @retval int Depth
 */
int RawVolumeLoader::Depth()    {
    return m_volume.Depth();
}

/**
 * Component取得
 * @retval int Component数
 */
int RawVolumeLoader::Component() {
    return m_volume.Component();
}

/**
 * データバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* RawVolumeLoader::Buffer() {
    return m_volume.Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *RawVolumeLoader::VolumeData()
{
    return &m_volume;
}

