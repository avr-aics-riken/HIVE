/**
 * @file TetraBuffer.h
 * Tetraバッファ
 */
#ifndef _TETRABUFFER_H_
#define _TETRABUFFER_H_

#include "BaseBuffer.h"

class TetraModel;

/**
 * Tetraバッファ
 */
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
    TetraBuffer(RenderDevice* render);
    ~TetraBuffer();

    bool Create(const TetraModel* model);
    void Render() const;
    void Clear();
    void Update();
};

#endif //_TETRABUFFER_H_

