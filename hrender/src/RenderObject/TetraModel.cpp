#include <string>
#include "TetraModel.h"
#include "BufferTetraData.h"

class TetraModel::Impl
{
public:
    Impl()
    {
        m_tetra = 0;
    }
    
    ~Impl()
    {
    }
    
    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool Create(BufferTetraData* tetradata)
    {
        if (!tetradata) {
            return false;
        }
        
        m_tetra = tetradata;
        
        return true;
    }
    
    BufferTetraData* GetTetra() const {
        return m_tetra;
    }
        
    const std::string& GetShader() const {
        return m_shaderfile;
    }

private:
    RefPtr<BufferTetraData> m_tetra;
    std::string m_shaderfile;
};

TetraModel::TetraModel() : RenderObject(TYPE_TETRA)
{
    m_imp = new Impl();
}

TetraModel::~TetraModel()
{
    delete m_imp;
}

bool TetraModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

bool TetraModel::Create(BufferTetraData* tetradata)
{
    return m_imp->Create(tetradata);
}

BufferTetraData* TetraModel::GetTetra() const
{
    return m_imp->GetTetra();
}

const std::string& TetraModel::GetShader() const
{
    return m_imp->GetShader();
}

