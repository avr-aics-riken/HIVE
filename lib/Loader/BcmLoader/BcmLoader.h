/**
 * @file BcmLoader.h
 * BCMデータローダー
 */
#ifndef _BCMLOADER_H_
#define _BCMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * BCMデータローダー
 */
class BCMLoader : public RefCount
{
public:
    BCMLoader();
    ~BCMLoader();
    void Clear();
    bool Load(const char* filename);
    int Width();
    int Height();
    int Depth();
    int Component();
    FloatBuffer* Buffer();

    BufferVolumeData *VolumeData();
private:
    BufferVolumeData m_volume;
};

#endif //_BCMLOADER_H_

