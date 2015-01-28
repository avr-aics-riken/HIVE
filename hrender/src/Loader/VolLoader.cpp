#include <stdio.h>
#include <string.h>
#include "VolLoader.h"
#include "SimpleVOL.h"

VOLLoader::VOLLoader()
{
    Clear();
}
VOLLoader::~VOLLoader()
{
    Clear();
}

void VOLLoader::Clear()
{
    m_volume.Clear();
}

bool VOLLoader::Load(const char* filename)
{
    Clear();

    SimpleVOL vol;
    const float* buf = vol.Load(filename);
    if (!buf) {
        printf("Failed to load VOL volume: %s\n", filename);
        return false;
    }
    m_volume.m_buffer = new FloatBuffer();
    m_volume.m_dim[0] = vol.GetDim(0);
    m_volume.m_dim[1] = vol.GetDim(1);
    m_volume.m_dim[2] = vol.GetDim(2);
    m_volume.m_comp = vol.GetComponent();
    const int fnum = m_volume.m_dim[0] * m_volume.m_dim[1] * m_volume.m_dim[2] * m_volume.m_comp;
    m_volume.m_buffer->Create(fnum);
    memcpy(m_volume.m_buffer->GetBuffer(), buf, fnum * sizeof(float));
    delete [] buf;

    return true;
}

int VOLLoader::Width()    {
    return m_volume.Width();
}

int VOLLoader::Height()   {
    return m_volume.Height();
}

int VOLLoader::Depth()    {
    return m_volume.Depth();
}

int VOLLoader::Component() {
    return m_volume.Component();
}

FloatBuffer* VOLLoader::Buffer() {
    return m_volume.Buffer();
}

BufferVolumeData *VOLLoader::VolumeData()
{
    return &m_volume;
}

