#include <string>
#include "PointModel.h"
#include "BufferPointData.h"

class PointModel::Impl
{
public:
    Impl()
    {
        m_point = 0;
    }
    ~Impl()
    {
        
    }

    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool Create(BufferPointData* pointdata)
    {
        m_point = pointdata;
        return true;
    }
    
    BufferPointData* GetPoint() const {
        return m_point;
    }
    
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

private:
    RefPtr<BufferPointData> m_point;
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

bool PointModel::Create(BufferPointData* pointdata)
{
    return m_imp->Create(pointdata);
}

BufferPointData* PointModel::GetPoint() const
{
    return m_imp->GetPoint();
}

const std::string& PointModel::GetShader() const
{
    return m_imp->GetShader();
}

