#ifndef _BUFFERLINEDATA_H_
#define _BUFFERLINEDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferLineData : public RefCount
{
private:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<FloatBuffer> m_radius;
    RefPtr<FloatBuffer> m_mat;
public:
    BufferLineData();
    BufferLineData(BufferLineData* inst);
    ~BufferLineData();
    void Create(int vertexnum);
    void Clear();
    Vec3Buffer*  Position();
    FloatBuffer* Radius();
    FloatBuffer* Material();
    Vec3Buffer*  Position() const;
    FloatBuffer* Radius()   const;
    FloatBuffer* Material() const;
};

#endif //_BUFFERLINEDATA_H_

