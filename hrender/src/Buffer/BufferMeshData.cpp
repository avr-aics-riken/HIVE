#include "BufferMeshData.h"
#include "Buffer.h"
#include <vector>

class BufferMeshData::Impl
{
private:
    RefPtr<Vec3Buffer >  m_pos;
    RefPtr<Vec3Buffer >  m_normal;
    RefPtr<Vec2Buffer >  m_texcoord;
    RefPtr<UintBuffer >  m_index;
    RefPtr<FloatBuffer>  m_mat;
public:
    Impl()
    {
    }
    
    Impl(BufferMeshData* inst)
    {
        this->m_pos      = inst->Position();
        this->m_normal   = inst->Normal();
        this->m_texcoord = inst->Texcoord();
        this->m_index    = inst->Index();
        this->m_mat      = inst->Material();
    }
    
    ~Impl()
    {
    }
    
    void Create(int vertexnum, int indexnum)
    {
        m_pos      = new Vec3Buffer();
        m_normal   = new Vec3Buffer();
        m_texcoord = new Vec2Buffer();
        m_mat      = new FloatBuffer();
        m_index    = new UintBuffer();
        m_pos->Create(vertexnum);
        m_normal->Create(vertexnum);
        m_texcoord->Create(vertexnum);
        m_mat->Create(vertexnum);
        m_index->Create(indexnum);
    }
    
    void Clear()
    {
        m_pos      = 0;
        m_normal   = 0;
        m_texcoord = 0;
        m_index    = 0;
        m_mat      = 0;
    }
    
    void print()
    {
        /*	printf("Position() %08p\n", m_pos.Get()      );
         printf("Normal()   %08X\n", m_normal.Get()   );
         printf("Texcoord() %08X\n", m_texcoord.Get() );
         printf("Index()    %08X\n", m_index.Get()    );
         printf("Material() %08X\n", m_mat.Get()      );*/
    }
    
    Vec3Buffer  *Position() { return m_pos;      }
    Vec3Buffer  *Normal()   { return m_normal;   }
    Vec2Buffer  *Texcoord() { return m_texcoord; }
    UintBuffer  *Index()    { return m_index;    }
    FloatBuffer *Material() { return m_mat;      }
};

/// constructor
BufferMeshData::BufferMeshData()
{
    m_imp = new BufferMeshData::Impl();
}

/// constructor
BufferMeshData::BufferMeshData(BufferMeshData* inst)
{
    m_imp = new BufferMeshData::Impl(inst);
}

/// destructor
BufferMeshData::~BufferMeshData()
{
    delete m_imp;
}

void BufferMeshData::Create(int vertexnum, int indexnum)
{
    m_imp->Create(vertexnum, indexnum);
}

void BufferMeshData::Clear()
{
    m_imp->Clear();
}

void BufferMeshData::print()
{
    m_imp->print();
}

Vec3Buffer  *BufferMeshData::Position() { return m_imp->Position(); }
Vec3Buffer  *BufferMeshData::Normal()   { return m_imp->Normal();   }
Vec2Buffer  *BufferMeshData::Texcoord() { return m_imp->Texcoord(); }
UintBuffer  *BufferMeshData::Index()    { return m_imp->Index();    }
FloatBuffer *BufferMeshData::Material() { return m_imp->Material(); }

