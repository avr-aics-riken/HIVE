/**
 * @file SolidBuffer.h
 * Solidバッファ
 */
#ifndef _SOLIDBUFFER_H_
#define _SOLIDBUFFER_H_

#include "BaseBuffer.h"

class SolidModel;

/**
 * Solidバッファ
 */
class SolidBuffer : public BaseBuffer
{
private:
    unsigned int m_vtxnum     ;
    unsigned int m_vtx_id     ;
    unsigned int m_material_id;

    //SOLID DATA
    unsigned int m_tetranum;
    RefPtr<const SolidModel> m_model;
    
public:
    SolidBuffer(RENDER_MODE mode);
    ~SolidBuffer();

    bool Create(const SolidModel* model);
    void Render() const;
    void Clear();
    void Update();
};

#endif //_SOLIDBUFFER_H_

