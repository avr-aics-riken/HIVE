#ifndef _CDMLOADER_H_
#define _CDMLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

class CDMLoader : public RefCount
{
public:
    CDMLoader();
    ~CDMLoader();
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

#endif //_CDMLOADER_H_

