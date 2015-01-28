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
    m_dim[0] = m_dim[1] = m_dim[2] = 0;
    m_comp = 0;
    m_buffer = 0;
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
    m_buffer = new FloatBuffer();
    m_dim[0] = vol.GetDim(0);
    m_dim[1] = vol.GetDim(1);
    m_dim[2] = vol.GetDim(2);
    m_comp = vol.GetComponent();
    const int fnum = m_dim[0] * m_dim[1] * m_dim[2] * m_comp;
    m_buffer->Create(fnum);
    memcpy(m_buffer->GetBuffer(), buf, fnum * sizeof(float));
    delete [] buf;

    return true;
}

