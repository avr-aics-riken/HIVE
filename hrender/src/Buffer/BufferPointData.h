#ifndef _BUFFERPOINTDATA_H_
#define _BUFFERPOINTDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferPointData : public RefCount
{
public:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<FloatBuffer> m_radius;
    RefPtr<FloatBuffer> m_mat;
    BufferPointData();
    BufferPointData(BufferPointData* inst);
    ~BufferPointData();
    void Clear();
    Vec3Buffer*  Position() ;
    FloatBuffer* Radius();
    FloatBuffer* Material() ;
};

#endif //_BUFFERPOINTDATA_H_

