#ifndef _TETRAMODEL_H_
#define _TETRAMODEL_H_

#include <string>
#include "RenderObject.h"

class BufferTetraData;

class TetraModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
    
public:
	TetraModel();
    ~TetraModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferTetraData *Tetra);
    BufferTetraData* GetTetra() const;
    const std::string& GetShader() const;
};

#endif //_TETRAMODEL_H_

