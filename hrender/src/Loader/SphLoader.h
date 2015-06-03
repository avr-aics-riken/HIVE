/**
 * @file SPHLoader.h
 * SPHデータローダー
 */
#ifndef _SPHLOADER_H_
#define _SPHLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * SPHデータローダー
 */
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

    void Origin(float origin[3]) {
      origin[0] = m_origin[0];
      origin[1] = m_origin[1];
      origin[2] = m_origin[2];
    }

    void Pitch(float pitch[3]) {
      pitch[0] = m_pitch[0];
      pitch[1] = m_pitch[1];
      pitch[2] = m_pitch[2];
    }

    float Step() {
      return m_step;
    }

    float Time() {
      return m_time;
    }

    FloatBuffer* Buffer();
    BufferVolumeData *VolumeData();
private:
    RefPtr<BufferVolumeData> m_volume;
    float m_origin[3];
    float m_pitch[3];
    float m_step;
    float m_time;
};

#endif //_SPHLOADER_H_

