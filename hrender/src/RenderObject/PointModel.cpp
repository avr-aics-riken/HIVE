#include <string>
#include "PointModel.h"

class PointModel::Impl
{
public:
    Impl()
    {
        
    }
    ~Impl()
    {
        
    }

    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool Create(BufferPointData* volume)
    {
        m_volume   = volume;
        return true;
    }
    
    BufferPointData* GetPoint() const {
        return m_volume;
    }
    
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

private:
    BufferPointData* m_volume;
    std::string m_shaderfile;

};
PointModel::PointModel() : RenderObject(TYPE_POINT)
{
    m_imp = new Impl();
}

PointModel::~PointModel()
{
    delete m_imp;
}

bool PointModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

bool PointModel::Create(BufferPointData* volume)
{
    return m_imp->Create(volume);
}

BufferPointData* PointModel::GetPoint() const
{
    return m_imp->GetPoint();
}

const std::string& PointModel::GetShader() const
{
    return m_imp->GetShader();
}

