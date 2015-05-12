/**
 * @file RawVolumeSaver.h
 * RawVolumeSaverクラス
 */
#ifndef _HIVE_RAWVOLUMESAVER_H_
#define _HIVE_RAWVOLUMESAVER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * RawVolumeSaverクラス
 */
class RawVolumeSaver : public RefCount
{
public:
    RawVolumeSaver();
    ~RawVolumeSaver();
    bool Save(const char* filename, bool appendFooter);

    void SetVolumeData(const BufferVolumeData* volumeData) {
      m_volumeData = volumeData;
    }

private:

    const BufferVolumeData* m_volumeData;

};

#endif //_HIVE_RAWVOLUMESAVER_H_

