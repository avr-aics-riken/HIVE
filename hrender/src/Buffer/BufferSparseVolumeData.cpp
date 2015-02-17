#include "BufferSparseVolumeData.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

class BufferSparseVolumeData::Impl
{
private:
    int m_dim[3];
    int m_comp;
    std::vector<VolumeBlock> m_volumes;

public:
    
    Impl()
    {
    }
    
    Impl(BufferSparseVolumeData* inst)
    {
    }
    
    ~Impl()
    {
    }
    
    void Create(int w, int h, int d, int component)
    {
    }
    
    void AddVolume(int offset_x, int offset_y, int offset_z,
                                           BufferVolumeData* vol)
    {
    }
    
    void Clear()
    {
    }
    
    void print()
    {
    }
    
    int Width()
    {
        return -1; // @todo
    }
    
    int Height()
    {
        return -1; // @todo
    }
    
    int Depth()
    {
        return -1; // @todo
    }
    
    int Component()
    {
        return 1; // @todo
    }
    
    const std::vector<BufferSparseVolumeData::VolumeBlock>& Buffers() const
    {
        // @todo
        static std::vector<BufferSparseVolumeData::VolumeBlock> tmp;
        return tmp;
    }
    
    std::vector<BufferSparseVolumeData::VolumeBlock>& Buffers()
    {
        // @todo
        static std::vector<BufferSparseVolumeData::VolumeBlock> tmp;
        return tmp;
    }
};

BufferSparseVolumeData::BufferSparseVolumeData()
{
    m_imp = new BufferSparseVolumeData::Impl();
}

BufferSparseVolumeData::BufferSparseVolumeData(BufferSparseVolumeData* inst)
{
    m_imp = new BufferSparseVolumeData::Impl(inst);
}

BufferSparseVolumeData::~BufferSparseVolumeData()
{
    delete m_imp;
}

void BufferSparseVolumeData::Create(int w, int h, int d, int component)
{
    m_imp->Create(w, h, d, component);
}

void BufferSparseVolumeData::AddVolume(int offset_x, int offset_y, int offset_z,
               BufferVolumeData* vol)
{
    m_imp->AddVolume(offset_x, offset_y, offset_z, vol);
}

void BufferSparseVolumeData::Clear()
{
    m_imp->Clear();
}

void BufferSparseVolumeData::print()
{
    m_imp->print();
}

int BufferSparseVolumeData::Width()
{
    return m_imp->Width();
}

int BufferSparseVolumeData::Height()
{
    return m_imp->Height();
}

int BufferSparseVolumeData::Depth()
{
    return m_imp->Depth();
}

int BufferSparseVolumeData::Component()
{
    return m_imp->Component();
}

const std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::Buffers() const
{
    return m_imp->Buffers();
}

std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::Buffers()
{
    return m_imp->Buffers();
}

