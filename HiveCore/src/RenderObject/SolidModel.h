/**
 * @file SolidModel.h
 * Solid モデル
 */
#ifndef _SOLIDMODEL_H_
#define _SOLIDMODEL_H_

#include <string>
#include "RenderObject.h"

class BufferSolidData;

/**
 * Solid モデル
 */
class SolidModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    SolidModel();
    ~SolidModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferSolidData *Solid);
    BufferSolidData* GetSolid() const;
    const std::string& GetShader() const;
    
    virtual bool GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const;
};

#endif //_SOLIDMODEL_H_

