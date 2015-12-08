/**
 * @file PolygonModel.h
 * ポリゴンモデル
 */
#ifndef _POLYGONMODEL_H_
#define _POLYGONMODEL_H_

#include <string>
#include "RenderObject.h"

class BufferMeshData;

/**
 * ポリゴンモデル
 */
class PolygonModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
    
public:
	PolygonModel();
    ~PolygonModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferMeshData *mesh);
    BufferMeshData* GetMesh() const;
    const std::string& GetShader() const;

};

#endif //_POLYGONMODEL_H_

