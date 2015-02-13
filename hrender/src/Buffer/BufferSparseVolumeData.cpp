#include "BufferSparseVolumeData.h"

BufferSparseVolumeData::BufferSparseVolumeData()
{
    
}
BufferSparseVolumeData::BufferSparseVolumeData(BufferSparseVolumeData* inst)
{
    
}
BufferSparseVolumeData::~BufferSparseVolumeData()
{
    
}
void BufferSparseVolumeData::Create(int w, int h, int d, int component)
{
    
}
void BufferSparseVolumeData::AddVolume(int offset_x, int offset_y, int offset_z,
               BufferVolumeData* vol)
{
    
}
void BufferSparseVolumeData::Clear()
{
    
}

void BufferSparseVolumeData::print()
{
    
}
int BufferSparseVolumeData::Width()
{
    return -1; // @todo    
}
int BufferSparseVolumeData::Height()
{
    return -1; // @todo    
}
int BufferSparseVolumeData::Depth()
{
    return -1; // @todo    
}
int BufferSparseVolumeData::Component()
{
    return 1; // @todo   
}
const std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::Buffers() const
{
    // @todo
    static std::vector<BufferSparseVolumeData::VolumeBlock> tmp;
    return tmp;
}
std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::Buffers()
{
    // @todo
    static std::vector<BufferSparseVolumeData::VolumeBlock> tmp;
    return tmp;
}

