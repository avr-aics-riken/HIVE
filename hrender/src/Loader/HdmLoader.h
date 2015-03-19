/**
 * @file HdmLoader.h
 * HDMデータローダー
 */
#ifndef _HDMLOADER_H_
#define _HDMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * HDMデータローダー
 */
class HDMLoader : public RefCount
{
public:
    HDMLoader();
    ~HDMLoader();
    void Clear();
    bool Load(const char* cellidFilename, const char* dataFilename);
    int Width();
    int Height();
    int Depth();
    int Component();
    FloatBuffer* Buffer();

    BufferVolumeData *VolumeData();
private:
    BufferVolumeData m_volume;
};

#endif //_HDMLOADER_H_
