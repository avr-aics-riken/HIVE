#include <stdio.h>
#include <string.h>
#include "SphLoader.h"
#include "SimpleSPH.h"
#include "BufferVolumeData.h"

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
    m_volume = 0;
}

bool SPHLoader::Load(const char* filename)
{
    Clear();
    m_volume = new BufferVolumeData();

    SimpleSPH sph;
    const float* buf = sph.Load(filename);
    if (!buf) {
        printf("Failed to load SPH volume: %s\n", filename);
        return false;
    }
    const int w = sph.GetDim(0);
    const int h = sph.GetDim(1);
    const int d = sph.GetDim(2);
    const int c = sph.GetComponent();
    m_volume->Create(w, h, d, c);
    FloatBuffer* buffer = m_volume->Buffer();
    const int fnum = w * h * d * c;
    memcpy(buffer->GetBuffer(), buf, fnum * sizeof(float));
    delete [] buf;

    return true;
}

int SPHLoader::Width()    {
    return m_volume->Width();
}

int SPHLoader::Height()   {
    return m_volume->Height();
}

int SPHLoader::Depth()    {
    return m_volume->Depth();
}

int SPHLoader::Component() {
    return m_volume->Component();
}

FloatBuffer* SPHLoader::Buffer() {
    return m_volume->Buffer();
}

BufferVolumeData *SPHLoader::VolumeData()
{
    return m_volume;
}
