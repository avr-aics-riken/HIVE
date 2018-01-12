/**
 * @file LineBuffer.h
 * ラインバッファ
 */
#ifndef _LINEBUFFER_H_
#define _LINEBUFFER_H_

#include "BaseBuffer.h"

class LineModel;

/**
 * ラインバッファ
 */
class LineBuffer : public BaseBuffer
{
private:
    unsigned int m_vtxnum     ;
    unsigned int m_indexnum   ;
    unsigned int m_vtx_id     ;
    unsigned int m_radius_id  ;
    unsigned int m_material_id;
    unsigned int m_idx_id     ;
    RefPtr<const LineModel> m_model;
    
public:
    LineBuffer(RenderPlugin* render);
    ~LineBuffer();

    bool Create(const LineModel* point);
    void Render() const;
    void Clear();
    void Update();
};

#endif //_LINEBUFFER_H_

