#ifndef _MANDELBULBVOLGEN_H_
#define _MANDELBULBVOLGEN_H_

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

/**
 * Mandelbulb Volume Generator
 */
class MandelbulbVolGen : public RefCount
{
public:
    MandelbulbVolGen();
    ~MandelbulbVolGen();
    void Clear();
    bool Generate(const int size);
    bool Generate(const int size,
                  const float originX, const float originY, const float originZ);
    int Width();
    int Height();
    int Depth();
    int Component();
    FloatBuffer* Buffer();

    BufferVolumeData *VolumeData();
private:
    RefPtr<BufferVolumeData> m_volume;
};

#endif //_MANDELBULBVOLGEN_H_
