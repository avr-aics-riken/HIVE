#include <string>
#include "LineModel.h"
#include "BufferLineData.h"

class LineModel::Impl
{
public:
    Impl()
    {
        m_line = 0;
        m_width = 1.0f;
    }
    ~Impl()
    {
        
    }

    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    bool SetLineWidth(float width)
    {
        m_width = width;
        return true;
    }
    
    bool Create(BufferLineData* Linedata)
    {
        m_line = Linedata;
        return true;
    }
    
    BufferLineData* GetLine() const {
        return m_line;
    }
    
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

    float GetLineWidth() const
    {
        return m_width;
    }

private:
    RefPtr<BufferLineData> m_line;
    float m_width;
    std::string m_shaderfile;

};
LineModel::LineModel() : RenderObject(TYPE_LINE)
{
    m_imp = new Impl();
}

LineModel::~LineModel()
{
    delete m_imp;
}

bool LineModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

bool LineModel::SetLineWidth(float width)
{
    return m_imp->SetLineWidth(width);
}

bool LineModel::Create(BufferLineData* Linedata)
{
    return m_imp->Create(Linedata);
}

BufferLineData* LineModel::GetLine() const
{
    return m_imp->GetLine();
}

const std::string& LineModel::GetShader() const
{
    return m_imp->GetShader();
}

float LineModel::GetLineWidth() const
{
    return m_imp->GetLineWidth();
}

