#include <string>
#include "PolygonModel.h"
#include "BufferMeshData.h"

class PolygonModel::Impl
{
public:
    Impl()
    {
        m_mesh = 0;
    }
    
    ~Impl()
    {
    }
    
    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool Create(BufferMeshData* m)
    {
        m_mesh = m;
        return true;
    }
    
    BufferMeshData* GetMesh() const {
        return m_mesh;
    }
    
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }
    
private:
    RefPtr<BufferMeshData> m_mesh;
    std::string m_shaderfile;
};

PolygonModel::PolygonModel() : RenderObject(TYPE_POLYGON)
{
    m_imp = new Impl();
}

PolygonModel::~PolygonModel()
{
    delete m_imp;
}

bool PolygonModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

bool PolygonModel::Create(BufferMeshData* m)
{
    return m_imp->Create(m);
}

BufferMeshData* PolygonModel::GetMesh() const
{
    return m_imp->GetMesh();
}

const std::string& PolygonModel::GetShader() const
{
    return m_imp->GetShader();
}
