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
    RefPtr<UintBuffer>  m_index;
    
public:
    BufferLineData();
    BufferLineData(BufferLineData* inst);
    ~BufferLineData();
    void Create(int vertexnum, int indexnum);
    void Clear();
    Vec3Buffer*  Position();
    FloatBuffer* Radius();
    FloatBuffer* Material();
    UintBuffer* Index();
    Vec3Buffer*  Position() const;
    FloatBuffer* Radius()   const;
    FloatBuffer* Material() const;
    UintBuffer*  Index()    const;
};

#endif //_BUFFERLINEDATA_H_

