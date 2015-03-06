#include "BufferTetraData.h"
#include "Buffer.h"
#include <vector>

class BufferTetraData::Impl
{
private:
    RefPtr<Vec3Buffer >  m_pos;
    RefPtr<UintBuffer >  m_index;
public:
    Impl()
    {
        Clear();
    }
    
    Impl(BufferTetraData* inst)
    {
        this->m_pos      = inst->Position();
        this->m_index    = inst->Index();
    }
    
    ~Impl()
    {
        m_pos      = 0;
        m_index    = 0;
    }
    
    void Create(int vertexnum, int indexnum)
    {
        m_pos->Create(vertexnum);
        m_index->Create(indexnum);
    }
    
    void Clear()
    {
        m_pos      = new Vec3Buffer();
        m_index    = new UintBuffer();
    }
    
    void print()
    {
        /*	printf("Position() %08p\n", m_pos.Get()      );
         printf("Index()    %08X\n", m_index.Get()    );*/
    }
    
    Vec3Buffer  *Position() { return m_pos;      }
    UintBuffer  *Index()    { return m_index;    }
};

/// constructor
BufferTetraData::BufferTetraData()
{
    m_imp = new BufferTetraData::Impl();
}

/// constructor
BufferTetraData::BufferTetraData(BufferTetraData* inst)
{
    m_imp = new BufferTetraData::Impl(inst);
}

/// destructor
BufferTetraData::~BufferTetraData()
{
    delete m_imp;
}

void BufferTetraData::Create(int vertexnum, int indexnum)
{
    m_imp->Create(vertexnum, indexnum);
}

void BufferTetraData::Clear()
{
    m_imp->Clear();
}

void BufferTetraData::print()
{
    m_imp->print();
}

Vec3Buffer  *BufferTetraData::Position() { return m_imp->Position(); }
UintBuffer  *BufferTetraData::Index()    { return m_imp->Index();    }

