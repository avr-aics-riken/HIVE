/**
 * @file HdmLoader.h
 * HDMデータローダー
 */
#ifndef _HDMLOADER_H_
#define _HDMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferSparseVolumeData.h"

/**
 * HDMデータローダー
 */
class HDMLoader : public RefCount
{
  public:
	HDMLoader();
	~HDMLoader();
	void Clear();
	bool Load(const char *cellidFilename, const char *dataFilename,
			  const char *fieldName, const char *fieldType, int components,
			  int virtualCells = 2);
	int Width();
	int Height();
	int Depth();
	int Component();
	FloatBuffer *Buffer();

	BufferSparseVolumeData *SparseVolumeData();

  private:
	BufferSparseVolumeData m_sparseVolume;
};

#endif //_HDMLOADER_H_
