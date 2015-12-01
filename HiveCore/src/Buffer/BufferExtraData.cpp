#include "BufferExtraData.h"
#include "Buffer.h"
#include <vector>
#include <string>
#include <algorithm>

class BufferExtraData::Impl
{
private:
    RefPtr<class Vec4Buffer>   m_vec4buffer;
    RefPtr<class Vec3Buffer>   m_vec3buffer;
    RefPtr<class Vec2Buffer>   m_vec2buffer;
    RefPtr<class FloatBuffer>  m_floatbuffer;
    RefPtr<class UintBuffer>   m_uintbuffer;

public:
    Impl()
    {
    }

    Impl(const BufferExtraData* inst)
    {
        m_vec4buffer  = inst->m_imp->m_vec4buffer;
        m_vec3buffer  = inst->m_imp->m_vec3buffer;
        m_vec2buffer  = inst->m_imp->m_vec2buffer;
        m_floatbuffer = inst->m_imp->m_floatbuffer;
        m_uintbuffer  = inst->m_imp->m_uintbuffer;
    }
  
    void Create(const char* dataType, int num)
    {
        std::string dtype(dataType);
        std::transform(dtype.begin(), dtype.end(), dtype.begin(), ::tolower);
        if (dtype == "float") {
            m_floatbuffer = new class FloatBuffer();
            m_floatbuffer->Create(num);
        } else if (dtype == "vec4") {
            m_vec4buffer = new class Vec4Buffer();
            m_vec4buffer->Create(num);
        } else if (dtype == "vec3") {
            m_vec3buffer = new class Vec3Buffer();
            m_vec3buffer->Create(num);
        } else if (dtype == "vec2") {
            m_vec2buffer = new class Vec2Buffer();
            m_vec2buffer->Create(num);
        } else if (dtype == "uint") {
            m_uintbuffer = new class UintBuffer();
            m_uintbuffer->Create(num);
        }
    }
    const char* GetDataType() const
    {
        if (m_floatbuffer) {
            return "float";
        } else if (m_vec4buffer) {
            return "vec4";
        } else if (m_vec3buffer) {
            return "vec3";
        } else if (m_vec2buffer) {
            return "vec2";
        } else if (m_uintbuffer) {
            return "uint";
        }
        return "unknown";
    }
    void Clear()
    {
        m_vec4buffer  = 0;
        m_vec3buffer  = 0;
        m_vec2buffer  = 0;
        m_floatbuffer = 0;
        m_uintbuffer  = 0;
    }
    
    int GetNum() const
    {
        if (m_floatbuffer) {
            return m_floatbuffer->GetNum();
        } else if (m_vec4buffer) {
            return m_vec4buffer->GetNum();
        } else if (m_vec3buffer) {
            return m_vec3buffer->GetNum();
        } else if (m_vec2buffer) {
            return m_vec2buffer->GetNum();
        } else if (m_uintbuffer) {
            return m_uintbuffer->GetNum();
        }
        return 0;
    }
    
    UintBuffer*        Uint()        { return m_uintbuffer;  }
    const UintBuffer*  Uint()  const { return m_uintbuffer;  }
    FloatBuffer*       Float()       { return m_floatbuffer; }
    const FloatBuffer* Float() const { return m_floatbuffer; }
    Vec4Buffer*        Vec4()        { return m_vec4buffer;  }
    const Vec4Buffer*  Vec4()  const { return m_vec4buffer;  }
    Vec3Buffer*        Vec3()        { return m_vec3buffer;  }
    const Vec3Buffer*  Vec3()  const { return m_vec3buffer;  }
    Vec2Buffer*        Vec2()        { return m_vec2buffer;  }
    const Vec2Buffer*  Vec2()  const { return m_vec2buffer;  }

};

BufferExtraData::BufferExtraData() : BufferData(TYPE_EXTRA), m_imp(new Impl()) {}

BufferExtraData::BufferExtraData(const BufferExtraData* inst) : BufferData(TYPE_EXTRA)
{
    m_imp = new BufferExtraData::Impl(inst);
}

BufferExtraData::~BufferExtraData()
{
    delete m_imp;
}

void BufferExtraData::Create(const char* dataType, int num)
{
    m_imp->Create(dataType, num);
}

void BufferExtraData::Clear()
{
    m_imp->Clear();
}

int BufferExtraData::GetNum() const
{
    return m_imp->GetNum();
}

const char* BufferExtraData::GetDataType() const
{
    return m_imp->GetDataType();
}

FloatBuffer* BufferExtraData::Float()
{
    return m_imp->Float();
}
UintBuffer* BufferExtraData::Uint()
{
    return m_imp->Uint();
}
Vec4Buffer* BufferExtraData::Vec4()
{
    return m_imp->Vec4();
}
Vec3Buffer* BufferExtraData::Vec3()
{
    return m_imp->Vec3();
}
Vec2Buffer* BufferExtraData::Vec2()
{
    return m_imp->Vec2();
}


const FloatBuffer* BufferExtraData::Float() const
{
    return m_imp->Float();
}
const UintBuffer* BufferExtraData::Uint() const
{
    return m_imp->Uint();
}
const Vec4Buffer* BufferExtraData::Vec4() const
{
    return m_imp->Vec4();
}
const Vec3Buffer* BufferExtraData::Vec3() const
{
    return m_imp->Vec3();
}
const Vec2Buffer* BufferExtraData::Vec2() const
{
    return m_imp->Vec2();
}

