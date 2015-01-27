#ifndef _POLYGONMODEL_H_
#define _POLYGONMODEL_H_

#include "RenderObject.h"
#include "LuaUtil.h"

class BufferMeshData;

class PolygonModel : public RenderObject
{
private:
	BufferMeshData* m_mesh;
    std::string m_shaderfile;
public:
	PolygonModel();
    ~PolygonModel();
    bool SetShader(const std::string& shaderfile);
    bool Create(BufferMeshData *mesh);
    BufferMeshData* GetMesh() const;
    const std::string& GetShader() const;

};

#endif //_POLYGONMODEL_H_

