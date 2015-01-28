#ifndef _SPHLOADER_H_
#define _SPHLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

class SPHLoader : public RefCount
{
public:
    SPHLoader();
    ~SPHLoader();
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

#endif //_SPHLOADER_H_

