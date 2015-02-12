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
    // NOTE: Assume MPI_Init() was already called in hrender::main()
    //
    int GVoxel[3] = { 10, 5, 7 }; // @fixme.
    int GDiv[3]   = { 1, 1, 1 }; // @fixme.
    int head[3] = { 1, 1, 1 }; // @fixme.
    int tail[3] = { 10, 5, 7 }; // @fixme.
    int virtualCellSize = 1; // @fixme.
    unsigned step = 10; // timestep @fixme.


    std::string dfi_filename = std::string(filename);
    
    CDM::E_CDM_ERRORCODE ret = CDM::E_CDM_SUCCESS;
    cdm_DFI* DFI_IN = cdm_DFI::ReadInit(MPI_COMM_WORLD, dfi_filename, GVoxel, GDiv, ret);
    if (ret != CDM::E_CDM_SUCCESS || DFI_IN == NULL) {
        printf("Failed to load DFI file: %s\n", filename);
        return false;
    }

    // Check data type
    // @todo { Support more data type. }
    //if( (DFI_IN->GetDataType() == CDM::E_CDM_UINT8) ||
    //    (DFI_IN->GetDataType() == CDM::E_CDM_FLOAT32) ||
    //    (DFI_IN->GetDataType() == CDM::E_CDM_FLOAT64) ) {
    if (DFI_IN->GetDataType() == CDM::E_CDM_FLOAT32) {
        // OK
    } else {
        printf("Unsupported data type: %s\n", DFI_IN->GetDataTypeString().c_str());
        return CDM::E_CDM_ERROR;
    }

    int numVariables = DFI_IN->GetNumVariables();
    //printf("num variables = %d\n", numVariables);

    // Get unit
    std::string Lunit;
    double Lref,Ldiff;
    bool LBset;
    ret = DFI_IN->GetUnit("Length",Lunit,Lref,Ldiff,LBset);
    if( ret==CDM::E_CDM_SUCCESS ) {
        printf("Length\n");
        printf("  Unit      : %s\n",Lunit.c_str());
        printf("  reference : %e\n",Lref);
        if( LBset ) {
            printf("  difference: %e\n",Ldiff);
        }
    }

    float r_time;       ///<dfiから読込んだ時間
    unsigned i_dummy;   ///<平均化ステップ
    float f_dummy;      ///<平均時間

    // data size
    size_t dataSize = (GVoxel[0]+2*virtualCellSize)*(GVoxel[1]+2*virtualCellSize)*(GVoxel[2]+2*virtualCellSize);
    printf("dataSize: %d\n", dataSize);
    float* d_v = new float[dataSize];

    ret =  DFI_IN->ReadData(d_v,                ///< pointer to buffer
                            step,               ///< timestep
                            virtualCellSize,    ///< virtual cell size
                            GVoxel,             ///< global dim
                            GDiv,               ///< num divs
                            head,               ///< start time
                            tail,               ///< end time
                            r_time,             ///< dfi read time
                            true,               ///< don't read averate?
                            i_dummy,
                            f_dummy );
        
    if( ret != CDM::E_CDM_SUCCESS ) {
      printf("Error reading CDM data.");
      //delete [] d_v;
      delete DFI_IN;
      return false;
    }

    //
    // Create volume data by stripping virtual cells.
    //
#if 0
    m_volume.Create(GVoxel[0], GVoxel[1], GVoxel[2], numVariables); // @fixme
    float* dst = m_volume.Buffer()->GetBuffer();

    size_t sx = GVoxel[0] + 2 * virtualCellSize;
    size_t sy = GVoxel[1] + 2 * virtualCellSize;
    for (size_t z = 0; z < GVoxel[2]; z++) {
        for (size_t y = 0; y < GVoxel[1]; y++) {
            for (size_t x = 0; x < GVoxel[0]; x++) {
                for (size_t c = 0; c < numVariables; c++) {
                    size_t srcIdx = (z + virtualCellSize) * sy * sx + (y + virtualCellSize) + sx + (x + virtualCellSize);
                    size_t dstIdx = z * GVoxel[1] * GVoxel[0] + y * GVoxel[0] + x;
                    float val = d_v[numVariables * srcIdx + c];
                    dst[numVariables * dstIdx + c] = val;
                }
            }
        }
    }
#endif
    //delete [] d_v;
    delete DFI_IN;

    printf("DBG: CDM load OK\n");

    return true;
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

