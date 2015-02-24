#include <string>
#include "VectorModel.h"
#include "BufferVectorData.h"

class VectorModel::Impl
{
public:
    Impl()
    {
        m_vector      = 0;
        m_linewidth   = 0.5f;
        m_arrowsize   = 1.0f;
        m_lengthscale = 1.0f;
    }
    
    ~Impl()
    {
    }
    
    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool Create(BufferVectorData* vectordata)
    {
        if (!vectordata) {
            return false;
        }
        m_vector = vectordata;
        
        return true;
    }
    
    BufferVectorData* GetVector() const {
        return m_vector;
    }
    
    const std::string& GetShader() const {
        return m_shaderfile;
    }
    
    float GetLineWidth() const {
        return m_linewidth;
    }
    float SetLineWidth(float w) {
        return m_linewidth;
    }
    float GetArrowSize() const {
        return m_arrowsize;
    }
    void SetArrowSize(float w) {
        m_arrowsize = w;
    }
    
    float GetLengthScale() const {
        return m_lengthscale;
    }
    
    void SetLengthScale(float s) {
        m_lengthscale = s;
    }

private:
    RefPtr<BufferVectorData>  m_vector;
    float m_linewidth;
    float m_arrowsize;
    float m_lengthscale;
    std::string m_shaderfile;
};

VectorModel::VectorModel() : RenderObject(TYPE_VECTOR) 
{
    m_imp = new Impl();
}

VectorModel::~VectorModel()
{
    delete m_imp;
}

bool VectorModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

bool VectorModel::Create(BufferVectorData* vectordata)
{
    return m_imp->Create(vectordata);
}

BufferVectorData* VectorModel::GetVector()
{
    return m_imp->GetVector();
}

const BufferVectorData* VectorModel::GetVector() const
{
    return m_imp->GetVector();
}

const std::string& VectorModel::GetShader() const
{
    return m_imp->GetShader();
}

float VectorModel::GetLineWidth() const {
    return m_imp->GetLineWidth();
}

void VectorModel::SetLineWidth(float s) {
    m_imp->SetLineWidth(s);
}

float VectorModel::GetArrowSize() const {
    return m_imp->GetArrowSize();
}

void VectorModel::SetArrowSize(float s) {
    m_imp->SetArrowSize(s);
}

float VectorModel::GetLengthScale() const {
    return m_imp->GetLengthScale();
}

void VectorModel::SetLengthScale(float s) {
    m_imp->SetLengthScale(s);
}

