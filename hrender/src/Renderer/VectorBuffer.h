#ifndef _VECTORBUFFER_H_
#define _VECTORBUFFER_H_

#include "BaseBuffer.h"

class VectorModel;

class VectorBuffer : public BaseBuffer
{
private:
    //LINE DATA
    unsigned int m_vtxnum     ;
    unsigned int m_indexnum   ;
    unsigned int m_vtx_id     ;
    unsigned int m_radius_id  ;
    unsigned int m_material_id;
    unsigned int m_idx_id     ;
    //TETRA DATA
    unsigned int m_tetranum;
    RefPtr<const VectorModel> m_model;
    
public:
    VectorBuffer(RENDER_MODE mode);
    ~VectorBuffer();

    bool Create(const VectorModel* point);
    void Render() const;
    void Clear();
};

#endif //_VECTORBUFFER_H_

