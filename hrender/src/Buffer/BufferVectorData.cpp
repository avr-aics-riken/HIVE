#include "BufferVectorData.h"
#include "Buffer.h"
#include <vector>

class BufferVectorData::Impl
{
private:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<Vec3Buffer>  m_dir;
public:
    Impl()
    {
        Clear();
    }
    
    Impl(BufferVectorData* inst)
    {
        this->m_pos = inst->Position();
        this->m_dir = inst->Direction();
    }
    
    ~Impl()
    {
        m_pos = 0;
        m_dir = 0;
    }
    
    void Create(int vertexnum)
    {
        m_pos->Create(vertexnum);
        m_dir->Create(vertexnum);
    }
    
    void Clear()
    {
        m_pos = new Vec3Buffer();
        m_dir = new Vec3Buffer();
    }
    
    void print()
    {
        // TODO:
    }
    
    Vec3Buffer  *Position()  const { return m_pos; }
    Vec3Buffer  *Direction() const { return m_dir; }
};

/// constructor
BufferVectorData::BufferVectorData()
{
    m_imp = new BufferVectorData::Impl();
}

/// constructor
BufferVectorData::BufferVectorData(BufferVectorData* inst) : m_imp(new BufferVectorData::Impl(inst)) {}
/// destructor
BufferVectorData::~BufferVectorData()
{
    delete m_imp;
}

void BufferVectorData::Create(int vertexnum)
{
    m_imp->Create(vertexnum);
}

void BufferVectorData::Clear()
{
    m_imp->Clear();
}

void BufferVectorData::print()
{
    m_imp->print();
}

Vec3Buffer* BufferVectorData::Position()  { return m_imp->Position();  }
Vec3Buffer* BufferVectorData::Direction() { return m_imp->Direction(); }

const Vec3Buffer* BufferVectorData::Position()  const { return m_imp->Position();  }
const Vec3Buffer* BufferVectorData::Direction() const { return m_imp->Direction(); }

