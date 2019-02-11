/**
 * @file PolygonBuffer.h
 * ポリゴンバッファ
 */

#pragma once
/*
    PolygonBuffer.h
*/

#include "BaseBuffer.h"

class PolygonModel;

/**
 * ポリゴンバッファ
 */
class PolygonBuffer : public BaseBuffer
{
private:
    unsigned int m_vtx_id;
    unsigned int m_normal_id;
    unsigned int m_mat_id;
    unsigned int m_tex_id;
    unsigned int m_index_id;
    unsigned int m_vertex_num;
    unsigned int m_index_num;
    RefPtr<const PolygonModel> m_model;
    
public:
    PolygonBuffer(RenderDevice* render);
    ~PolygonBuffer();
    
    bool Create(const PolygonModel* model);
    void Render() const;
    void Clear();
    void Update();
};

