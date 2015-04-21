/**
 * @file VTKLoader.h
 * VTK Image(Volume data)ローダー
 */
#ifndef _VTKLOADER_H_
#define _VTKLOADER_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * VTKデータローダー
 */
class VTKLoader : public RefCount
{
public:
    VTKLoader();
    ~VTKLoader();
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

#endif //_VTKLOADER_H_

