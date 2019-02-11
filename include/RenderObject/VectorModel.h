/**
 * @file VectorModel.h
 * ベクトルモデル
 */
#ifndef _VECTORMODEL_H_
#define _VECTORMODEL_H_

#include <string>
#include "RenderObject.h"

class BufferVectorData;

/**
 * ベクトルモデル
 */
class VectorModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
    
public:
	VectorModel();
    ~VectorModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferVectorData *vector);
    const BufferVectorData* GetVector() const;
    BufferVectorData* GetVector();
    const std::string& GetShader() const;
    float GetLineWidth() const;
    void  SetLineWidth(float w);
    float GetArrowSize() const;
    void  SetArrowSize(float s);
    float GetLengthScale() const;
    void  SetLengthScale(float s);
    
	virtual bool GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const;
};

#endif //_VECTORMODEL_H_

