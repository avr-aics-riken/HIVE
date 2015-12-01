/**
 * @file RawVolumeLoader.h
 * RAW volume データローダー
 */
#ifndef _RAWVOLUMELOADER_H_
#define _RAWVOLUMELOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * Raw Volume データローダー
 */
class RawVolumeLoader : public RefCount
{
public:
    RawVolumeLoader();
    ~RawVolumeLoader();
    void Clear();

	/// Loads raw volume data.
    bool Load(const char* filename, int width, int height, int depth, int components, const char* format);

	int Width();
	int Height();
	int Depth();
	int Component();

    FloatBuffer* Buffer();


    BufferVolumeData *VolumeData();
private:
    BufferVolumeData m_volume;
};

#endif //_RAWVOLUMELOADER_H_

