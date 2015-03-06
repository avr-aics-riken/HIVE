#ifndef _TETRABUFFER_H_
#define _TETRABUFFER_H_

#include "BaseBuffer.h"

class TetraModel;

class TetraBuffer : public BaseBuffer
{
private:
    //LINE DATA
    unsigned int m_vtxnum     ;
    unsigned int m_vtx_id     ;
    unsigned int m_material_id;

    //TETRA DATA
    unsigned int m_tetranum;
    RefPtr<const TetraModel> m_model;
    
public:
    TetraBuffer(RENDER_MODE mode);
    ~TetraBuffer();

    bool Create(const TetraModel* model);
    void Render() const;
    void Clear();
};

#endif //_TETRABUFFER_H_

