#ifndef _BUFFERPOINTDATA_H_
#define _BUFFERPOINTDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferPointData : public RefCount
{
private:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<FloatBuffer> m_radius;
    RefPtr<FloatBuffer> m_mat;
public:
    BufferPointData();
    BufferPointData(BufferPointData* inst);
    ~BufferPointData();
    void Create(int vertexnum);
    void Clear();
    Vec3Buffer*  Position();
    FloatBuffer* Radius();
    FloatBuffer* Material();
    Vec3Buffer*  Position() const;
    FloatBuffer* Radius()   const;
    FloatBuffer* Material() const;
};

#endif //_BUFFERPOINTDATA_H_

