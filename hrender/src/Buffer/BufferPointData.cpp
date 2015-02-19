#include "BufferPointData.h"
#include "Buffer.h"
#include <vector>

class BufferPointData::Impl
{
private:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<FloatBuffer> m_radius;
    RefPtr<FloatBuffer> m_mat;
    
public:
    Impl()
    {
        Clear();
    }
    
    Impl(BufferPointData* inst)
    {
        this->m_pos    = inst->Position();
        this->m_mat    = inst->Material();
        this->m_radius = inst->Radius();
    }
    
    ~Impl()
    {
        m_pos    = 0;
        m_mat    = 0;
        m_radius = 0;
    }
    
    void Clear()
    {
        m_pos    = new Vec3Buffer();
        m_mat    = new FloatBuffer();
        m_radius = new FloatBuffer();
    }
    
    void Create(int vertexnum)
    {
        m_pos->Create(vertexnum);
        m_mat->Create(vertexnum);
        m_radius->Create(vertexnum);
    }
    
    Vec3Buffer* Position() {
        return m_pos;
    }
    
    FloatBuffer* Material() {
        return m_mat;
    }
    
    FloatBuffer* Radius() {
        return m_radius;
    }
    
    Vec3Buffer* Position() const {
        return m_pos;
    }
    
    FloatBuffer* Material() const {
        return m_mat;
    }
    
    FloatBuffer* Radius() const {
        return m_radius;
    }
};

/// constructor
BufferPointData::BufferPointData()
{
    m_imp = new BufferPointData::Impl();
}

/// constructor
BufferPointData::BufferPointData(BufferPointData* inst)
{
    m_imp = new BufferPointData::Impl(inst);
}

/// destructor
BufferPointData::~BufferPointData()
{
    delete m_imp;
}

void BufferPointData::Clear()
{
    m_imp->Clear();
}

void BufferPointData::Create(int vertexnum)
{
    m_imp->Create(vertexnum);
}

Vec3Buffer* BufferPointData::Position() {
    return m_imp->Position();
}

FloatBuffer* BufferPointData::Material() {
    return m_imp->Material();
}

FloatBuffer* BufferPointData::Radius() {
    return m_imp->Radius();
}

Vec3Buffer* BufferPointData::Position() const {
    return m_imp->Position();
}

FloatBuffer* BufferPointData::Material() const {
    return m_imp->Material();
}

FloatBuffer* BufferPointData::Radius() const {
    return m_imp->Radius();
}


