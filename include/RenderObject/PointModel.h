/**
 * @file PointModel.h
 * ポイントモデル
 */
#ifndef _POINTMODEL_H_
#define _POINTMODEL_H_

#include <string>
#include "RenderObject.h"


class BufferPointData;

/**
 * ポイントモデル
 */
class PointModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
public:
    PointModel();
    ~PointModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferPointData* volume);
    BufferPointData* GetPoint() const;
    const std::string& GetShader() const;

    virtual bool GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const;
};

#endif //_POINTMODEL_H_

