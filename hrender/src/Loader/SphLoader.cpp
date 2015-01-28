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
    m_dim[0] = m_dim[1] = m_dim[2] = 0;
    m_comp = 0;
    m_buffer = 0;
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
    m_buffer = new FloatBuffer();
    m_dim[0] = sph.GetDim(0);
    m_dim[1] = sph.GetDim(1);
    m_dim[2] = sph.GetDim(2);
    m_comp = sph.GetComponent();
    const int fnum = m_dim[0] * m_dim[1] * m_dim[2] * m_comp;
    m_buffer->Create(fnum);
    memcpy(m_buffer->GetBuffer(), buf, fnum*sizeof(float));
    delete [] buf;

    return true;
}

