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
    
    const int w = vol.GetDim(0);
    const int h = vol.GetDim(1);
    const int d = vol.GetDim(2);
    const int c = vol.GetComponent();
    m_volume.Create(w, h, d, c);
    memcpy(m_volume.Buffer()->GetBuffer(), buf, w * h * d * c * sizeof(float));
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

