//
// TODO: - timestep support
//       - support various volume data type
//

#ifndef HIVE_WITH_CDMLIB
#error "HIVE_WITH_CDMLIB must be defined when you compile CDMLoader module"
#endif

#include <mpi.h>

#include <stdio.h>
#include <string.h>

#include <string>
#include <fstream>

#include "CdmLoader.h"
#include "SimpleVOL.h"

#include "cdm_DFI.h"
#include "cdm_TextParser.h"

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

    if (!std::ifstream(filename).good()) {
        fprintf(stderr, "[CDMLoader] File not found: %s\n", filename);
        return false;
    }
        

    // Read info from .dfi
    std::string tpFilename(filename);
    std::string dirName = CDM::cdmPath_DirName(tpFilename); // path to index.dfi

    cdm_TextParser tp;
    tp.getTPinstance();
    {
        int error = tp.readTPfile(tpFilename);
        if (error) {
            fprintf(stderr, "[CDMLoader] Failed to read .dfi file: %s\n", tpFilename.c_str());
            return false;
        }
    }

    cdm_FileInfo fileInfo;
    {
        if (fileInfo.Read(tp) != CDM::E_CDM_SUCCESS) {
            fprintf(stderr, "[CDMLoader] Failed to read FileInfo from .dfi file: %s\n", tpFilename.c_str());
            return false;
        }
    }

    cdm_FilePath path;
    {
        if( path.Read(tp) != CDM::E_CDM_SUCCESS )
        {
            fprintf(stderr, "[CDMLoader] Failed to read FilePath from .dfi file: %s\n", tpFilename.c_str());
            return false;
        }
    }

    cdm_Unit unit;
    {
        if( unit.Read(tp) != CDM::E_CDM_SUCCESS )
        {
            fprintf(stderr, "[CDMLoader] Failed to read Unit Data from .dfi file: %s\n", tpFilename.c_str());
            return false;
        }
    }

    cdm_TimeSlice timeSlice;
    {
        if( timeSlice.Read(tp, fileInfo.FileFormat) != CDM::E_CDM_SUCCESS )
        {
            fprintf(stderr, "[CDMLoader] Failed to read TimeSlice Data from .dfi file: %s\n", tpFilename.c_str());
            return false;
        }
    }

    for (size_t i = 0; i < timeSlice.SliceList.size(); i++) {
        m_timeSteps.push_back(timeSlice.SliceList[i].step);
    } 

    //
    //  Remove current TextParser instance and swtich to read `proc.dfi`, then continue to read content of proc.dfi to get
    //  domain information.
    //
    tp.remove();

    std::string procfile = CDM::cdmPath_ConnectPath(dirName, CDM::cdmPath_FileName(path.ProcDFIFile, ".dfi"));
    if (!std::ifstream(procfile.c_str()).good()) {
        fprintf(stderr, "[CDMLoader] File not found: %s\n", procfile.c_str());
        return false;
    }

    {
        int error = tp.readTPfile(procfile);
        if (error) {
            fprintf(stderr, "[CDMLoader] Failed to read .dfi file: %s\n", procfile.c_str());
            return false;
        }
    }

    // Read Domain from proc.dfi
    cdm_Domain domain;
    {
        if (domain.Read(tp, dirName) != CDM::E_CDM_SUCCESS) {
            fprintf(stderr, "[CDMLoader] Failed to read Doamin info from .dfi file: %s\n", procfile.c_str());
            return false;
        }
    }

    tp.remove();

    m_globalVoxel[0] = domain.GlobalVoxel[0];
    m_globalVoxel[1] = domain.GlobalVoxel[1];
    m_globalVoxel[2] = domain.GlobalVoxel[2];

    m_globalDiv[0] = domain.GlobalDivision[0];
    m_globalDiv[1] = domain.GlobalDivision[1];
    m_globalDiv[2] = domain.GlobalDivision[2];

    m_globalOffset[0] = domain.GlobalOrigin[0];
    m_globalOffset[1] = domain.GlobalOrigin[1];
    m_globalOffset[2] = domain.GlobalOrigin[2];

    m_globalRegion[0] = domain.GlobalRegion[0];
    m_globalRegion[1] = domain.GlobalRegion[1];
    m_globalRegion[2] = domain.GlobalRegion[2];

    int GVoxel[3] = { m_globalVoxel[0], m_globalVoxel[1], m_globalVoxel[2] }; 
    int GDiv[3]   = { m_globalDiv[0], m_globalDiv[1], m_globalDiv[2] }; 
    int head[3] = { 1, 1, 1 }; // @fixme.
    int tail[3] = { m_globalVoxel[0], m_globalVoxel[1], m_globalVoxel[2] }; // @fixme.
    int virtualCellSize = 1; // @fixme.

    unsigned int step = 0; 

    // @fixme { Specify timestep from scene script }
    if (m_timeSteps.size() > 0) {
        step = m_timeSteps[0];
    }

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
        printf("[CdmLoader] Unsupported data type: %s\n", DFI_IN->GetDataTypeString().c_str());
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
    //printf("DBG: dataSize: %d\n", dataSize);
    float* d_v = new float[dataSize*numVariables];

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
      printf("[CdmLoader] Error reading CDM data.");
      //delete [] d_v;
      delete DFI_IN;
      return false;
    }

    //
    // Create volume data by stripping virtual cells.
    //
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

    delete [] d_v;
    delete DFI_IN;

    printf("[CdmLoader] CDM load OK\n");

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

