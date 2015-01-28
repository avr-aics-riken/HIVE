#ifndef _SPHLOADER_H_
#define _SPHLOADER_H_

#include "Ref.h"
#include "Buffer.h"

class SPHLoader : public RefCount
{
public:
    SPHLoader();
    ~SPHLoader();

    void Clear();
    bool Load(const char* filename);
    int Width()    {
        return m_dim[0];
    }
    int Height()   {
        return m_dim[1];
    }
    int Depth()    {
        return m_dim[2];
    }
    int Component() {
        return m_comp;
    }
    FloatBuffer* Buffer() {
        return m_buffer;
    }

private:
    int m_dim[3];
    int m_comp;
    RefPtr<FloatBuffer> m_buffer;
};

#endif //_SPHLOADER_H_

