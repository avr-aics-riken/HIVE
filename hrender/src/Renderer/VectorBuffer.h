/**
 * @file VectorBuffer.h
 * ベクトルバッファ
 */
#ifndef _VECTORBUFFER_H_
#define _VECTORBUFFER_H_

#include "BaseBuffer.h"

class VectorModel;

/**
 * ベクトルバッファクラス
 */
class VectorBuffer : public BaseBuffer
{
private:

    unsigned int m_line_vtx_id;
    unsigned int m_tetra_vtx_id;
    unsigned int m_mat_id;
    unsigned int m_lines_vnum;
    unsigned int m_tetra_vnum;

    RefPtr<const VectorModel> m_model;
    
public:
    VectorBuffer(RENDER_MODE mode);
    ~VectorBuffer();

    bool Create(const VectorModel* point);
    void Render() const;
    void Clear();
};

#endif //_VECTORBUFFER_H_

