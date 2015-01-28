#include <stdio.h>
#include <string.h>
#include "SphLoader.h"
#include "SimpleSPH.h"

SPHLoader::SPHLoader()
{
    Clear();
}

SPHLoader::~SPHLoader()
{
    Clear();
}

void SPHLoader::Clear()
{
    m_volume.Clear();
}

bool SPHLoader::Load(const char* filename)
{
    Clear();

    SimpleSPH sph;
    const float* buf = sph.Load(filename);
    if (!buf) {
        printf("Failed to load SPH volume: %s\n", filename);
        return false;
    }
    m_volume.m_buffer = new FloatBuffer();
    m_volume.m_dim[0] = sph.GetDim(0);
    m_volume.m_dim[1] = sph.GetDim(1);
    m_volume.m_dim[2] = sph.GetDim(2);
    m_volume.m_comp = sph.GetComponent();
    const int fnum = m_volume.m_dim[0] * m_volume.m_dim[1] * m_volume.m_dim[2] * m_volume.m_comp;
    m_volume.m_buffer->Create(fnum);
    memcpy(m_volume.m_buffer->GetBuffer(), buf, fnum*sizeof(float));
    delete [] buf;

    return true;
}

int SPHLoader::Width()    {
    return m_volume.Width();
}

int SPHLoader::Height()   {
    return m_volume.Height();
}

int SPHLoader::Depth()    {
    return m_volume.Depth();
}

int SPHLoader::Component() {
    return m_volume.Component();
}

FloatBuffer* SPHLoader::Buffer() {
    return m_volume.Buffer();
}

BufferVolumeData *SPHLoader::VolumeData()
{
    return &m_volume;
}
