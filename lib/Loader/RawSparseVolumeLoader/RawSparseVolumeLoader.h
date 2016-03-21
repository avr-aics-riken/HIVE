/**
 * @file RawSparseVolumeLoader.h
 * RAW sparse volume データローダー
 */
#ifndef _RAWSPARSEVOLUMELOADER_H_
#define _RAWSPARSEVOLUMELOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferSparseVolumeData.h"

/**
 * Raw Sparse Volume データローダー
 */
class RawSparseVolumeLoader : public RefCount
{
public:
    RawSparseVolumeLoader();
    ~RawSparseVolumeLoader();
    void Clear();

	/// Loads raw volume data.
  //  bool Load(const char* filename, int width, int height, int depth, int components, const char* format);

	/// Loads raw sparse volume data from memory(pointer).
  /// Assume opaque pointer `volumeData` is represented as `HIVESparseVolume`.
  bool LoadFromPointer(void* volumeData);

	//int Width();
	//int Height();
	//int Depth();
	//int Component();

  //  FloatBuffer* Buffer();

    BufferSparseVolumeData *SparseVolumeData();
private:
    RefPtr<BufferSparseVolumeData> m_sparseVolume;
};

#endif //_RAWSPARSEVOLUMELOADER_H_

