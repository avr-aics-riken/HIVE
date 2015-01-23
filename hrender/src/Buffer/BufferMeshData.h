#ifndef _BUFFERMESHDATA_H_
#define _BUFFERMESHDATA_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>

class BufferMeshData : public RefCount
{
public:
    RefPtr<Vec3Buffer >  m_pos;
    RefPtr<Vec3Buffer >  m_normal;
    RefPtr<Vec2Buffer >  m_texcoord;
    RefPtr<UintBuffer >  m_index;
    RefPtr<FloatBuffer>  m_mat;
    BufferMeshData();
    ~BufferMeshData();
    void Clear();
    Vec3Buffer  *Position() { return m_pos;      }
    Vec3Buffer  *Normal()   { return m_normal;   }
    Vec2Buffer  *Texcoord() { return m_texcoord; }
    UintBuffer  *Index()    { return m_index;    }
    FloatBuffer *Material() { return m_mat;      }
};

#endif //_BUFFERMESHDATA_H_
