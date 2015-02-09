#include <stdio.h>
#include <string.h>

#include <string>

#include "CDMLoader.h"
#include "SimpleVOL.h"

#ifndef HIVE_WITH_CDMLIB
#error "HIVE_WITH_CDMLIB must be defined when you compile CDMLoader module"
#endif

#include "cdm_DFI.h"

CDMLoader::CDMLoader()
{
    Clear();
}
CDMLoader::~CDMLoader()
{
    Clear();
}

void CDMLoader::Clear()
{
    m_volume.Clear();
}

bool CDMLoader::Load(const char* filename)
{
    Clear();

    //
    // NOTE: Assume MPI_Init() was already called in hrender
    //
    int GVoxel[3] = { 1, 1, 1 }; // @fixme.
    int GDiv[3]   = { 1, 1, 1 }; // @fixme.
    int head[3] = { 1, 1, 1 }; // @fixme.
    int tail[3] = { 1, 1, 1 }; // @fixme.
    int virtualCellSize = 1; // @fixme

    std::string dfi_filename = std::string(filename);
    
    CDM::E_CDM_ERRORCODE ret = CDM::E_CDM_SUCCESS;
    cdm_DFI* DFI_IN = cdm_DFI::ReadInit(MPI_COMM_WORLD, dfi_filename, GVoxel, GDiv, ret);
    if (ret != CDM::E_CDM_SUCCESS || DFI_IN == NULL) {
        printf("Failed to load DFI file: %s\n", filename);
        return false;
    }

    assert(0 && "TODO");
        
    //SimpleVOL vol;
    //const float* buf = vol.Load(filename);
    //if (!buf) {
    //    printf("Failed to load VOL volume: %s\n", filename);
    //    return false;
    //}
    //m_volume.m_buffer = new FloatBuffer();
    //m_volume.m_dim[0] = vol.GetDim(0);
    //m_volume.m_dim[1] = vol.GetDim(1);
    //m_volume.m_dim[2] = vol.GetDim(2);
    //m_volume.m_comp = vol.GetComponent();
    //const int fnum = m_volume.m_dim[0] * m_volume.m_dim[1] * m_volume.m_dim[2] * m_volume.m_comp;
    //m_volume.m_buffer->Create(fnum);
    //memcpy(m_volume.m_buffer->GetBuffer(), buf, fnum * sizeof(float));
    //delete [] buf;

    return false;
}

int CDMLoader::Width()    {
    return m_volume.Width();
}

int CDMLoader::Height()   {
    return m_volume.Height();
}

int CDMLoader::Depth()    {
    return m_volume.Depth();
}

int CDMLoader::Component() {
    return m_volume.Component();
}

FloatBuffer* CDMLoader::Buffer() {
    return m_volume.Buffer();
}

BufferVolumeData *CDMLoader::VolumeData()
{
    return &m_volume;
}

