/**
 * @file PointBuffer.h
 * ポイントバッファ
 */
#ifndef _POINTBUFFER_H_
#define _POINTBUFFER_H_

#include "BaseBuffer.h"

class PointModel;

/**
 * ポイントバッファ
 */
class PointBuffer : public BaseBuffer
{
private:
    unsigned int m_vtxnum     ;
    unsigned int m_vtx_id     ;
    unsigned int m_radius_id  ;
    unsigned int m_material_id;
    RefPtr<const PointModel> m_model;
    
public:
    PointBuffer(RENDER_MODE mode);
    ~PointBuffer();

    bool Create(const PointModel* point);
    void Render() const;
    void Clear();
};

#endif //_POINTBUFFER_H_

