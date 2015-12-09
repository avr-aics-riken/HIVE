/**
 * @file VolLoader.h
 * VOLデータローダー
 */
#ifndef _VOLLOADER_H_
#define _VOLLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * VOLデータローダー
 */
class VOLLoader : public RefCount
{
public:
    VOLLoader();
    ~VOLLoader();
    void Clear();
    bool Load(const char* filename);
    int Width();
    int Height();
    int Depth();
    int Component();
    FloatBuffer* Buffer();

    BufferVolumeData *VolumeData();
private:
    RefPtr<BufferVolumeData> m_volume;
};

#endif //_VOLLOADER_H_

