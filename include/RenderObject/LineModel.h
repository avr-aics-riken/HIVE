/**
 * @file LineModel.h
 * ラインモデル
 */
#ifndef _LINEMODEL_H_
#define _LINEMODEL_H_

#include <string>
#include "RenderObject.h"

class BufferLineData;

/**
 * ラインモデル
 */
class LineModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
public:
    LineModel();
    ~LineModel();
    bool SetShader(const std::string& shaderfile);
    bool SetLineWidth(float width);
    bool Create(BufferLineData* volume);
    BufferLineData* GetLine() const;
    const std::string& GetShader() const;
    float GetLineWidth() const;
    
    virtual bool GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const;
};

#endif //_LINEMODEL_H_

