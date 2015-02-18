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

    void GlobalOffset(double ret[3]) {
      ret[0] = m_globalOffset[0];
      ret[1] = m_globalOffset[1];
      ret[2] = m_globalOffset[2];
    }

    void GlobalRegion(double ret[3]) {
      ret[0] = m_globalRegion[0];
      ret[1] = m_globalRegion[1];
      ret[2] = m_globalRegion[2];
    }
  
    void GlobalVoxel(int ret[3]) {
      ret[0] = m_globalVoxel[0];
      ret[1] = m_globalVoxel[1];
      ret[2] = m_globalVoxel[2];
    }

    void GlobalDiv(int ret[3]) {
      ret[0] = m_globalDiv[0];
      ret[1] = m_globalDiv[1];
      ret[2] = m_globalDiv[2];
    }

    FloatBuffer* Buffer();

    BufferVolumeData *VolumeData();
private:
    BufferVolumeData m_volume;

    double m_globalOffset[3];
    double m_globalRegion[3];
    int    m_globalVoxel[3];
    int    m_globalDiv[3];
};

#endif //_CDMLOADER_H_

