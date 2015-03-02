#ifndef _HIVE_SPHSAVER_H_
#define _HIVE_SPHSAVER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

class SPHSaver : public RefCount
{
public:
    SPHSaver();
    ~SPHSaver();
    bool Save(const char* filename);

    void SetVolumeData(const BufferVolumeData* volumeData) {
      m_volumeData = volumeData;
    }

private:

    const BufferVolumeData* m_volumeData;

};

#endif //_HIVE_SPHSAVER_H_

