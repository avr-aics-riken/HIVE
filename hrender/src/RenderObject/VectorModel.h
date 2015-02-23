#ifndef _VECTORMODEL_H_
#define _VECTORMODEL_H_

#include <string>
#include "RenderObject.h"

class BufferLineData;
class BufferTetraData;

class VectorModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
    
public:
	VectorModel();
    ~VectorModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferLineData *Line, BufferTetraData *Tetra);
    BufferLineData* GetLine() const;
    BufferTetraData* GetTetra() const;
    const std::string& GetShader() const;

};

#endif //_VECTORMODEL_H_

