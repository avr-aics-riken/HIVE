/**
 * @file CdmLoader.cpp
 * CDMデータローダー
 */
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

/// コンストラクタ
CDMLoader::CDMLoader()
{
    Clear();
}

/// デストラクタ
CDMLoader::~CDMLoader()
{
    Clear();
}

/// ボリュームクリア
void CDMLoader::Clear()
{
    m_volume.Clear();
}

/**
 * CDMデータのロード
 * @param filename ファイルパス
 * @param virtualCell # of virtual cells
 * @param timeSliceIndex timeslice index
 * @retval true 成功
 * @retval false 失敗
 */
bool CDMLoader::Load(const char* filename, int virtualCells, int timeSliceIndex)
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

	bool isNonUniform = false;

    // Read Domain from proc.dfi
    cdm_Domain* domain;
	if (fileInfo.DFIType == CDM::E_CDM_DFITYPE_CARTESIAN) {
		domain = new cdm_Domain();
	} else if (fileInfo.DFIType == CDM::E_CDM_DFITYPE_NON_UNIFORM_CARTESIAN) {
		domain = new cdm_NonUniformDomain<double>();
		isNonUniform = true;
	} else {
		assert(0);
	}

    {
        if (domain->Read(tp, dirName) != CDM::E_CDM_SUCCESS) {
            fprintf(stderr, "[CDMLoader] Failed to read Doamin info from .dfi file: %s\n", procfile.c_str());
            return false;
        }
    }

    tp.remove();

	std::vector<float> coords[3];

	if (isNonUniform) {
		cdm_NonUniformDomain<double>* nuDomain = dynamic_cast<cdm_NonUniformDomain<double>*>(domain);

		coords[0].resize(nuDomain->GlobalVoxel[0] + 1);
		coords[1].resize(nuDomain->GlobalVoxel[1] + 1);
		coords[2].resize(nuDomain->GlobalVoxel[2] + 1);

		// Read coordinate data and normalize coordinate values to [0, 1]
		{
			for (size_t x = 0; x < nuDomain->GlobalVoxel[0] + 1; x++) {
				//printf("x[%d] = %f\n", x, nuDomain->NodeX(x));
				coords[0][x] = nuDomain->NodeX(x);
			}

			double minval = coords[0][0];
			double maxval = coords[0][nuDomain->GlobalVoxel[0]];

   			for (size_t x = 0; x < nuDomain->GlobalVoxel[0] + 1; x++) {
				coords[0][x] = (coords[0][x] - minval) / (maxval - minval);
			}
		}

		{
			for (size_t y = 0; y < nuDomain->GlobalVoxel[1] + 1; y++) {
				//printf("y[%d] = %f\n", y, nuDomain->NodeY(y));
				coords[2][y] = nuDomain->NodeY(y);
			}

			double minval = coords[1][0];
			double maxval = coords[1][nuDomain->GlobalVoxel[1]];

   			for (size_t x = 0; x < nuDomain->GlobalVoxel[1] + 1; x++) {
				coords[1][x] = (coords[1][x] - minval) / (maxval - minval);
			}
		}

		{
			for (size_t z = 0; z < nuDomain->GlobalVoxel[2] + 1; z++) {
				//printf("z[%d] = %f\n", z, nuDomain->NodeZ(z));
				coords[2][z] = nuDomain->NodeZ(z);
			}

			double minval = coords[2][0];
			double maxval = coords[2][nuDomain->GlobalVoxel[2]];

   			for (size_t x = 0; x < nuDomain->GlobalVoxel[2] + 1; x++) {
				coords[2][x] = (coords[2][x] - minval) / (maxval - minval);
			}

		}


	}
		

    m_globalVoxel[0] = domain->GlobalVoxel[0];
    m_globalVoxel[1] = domain->GlobalVoxel[1];
    m_globalVoxel[2] = domain->GlobalVoxel[2];

    m_globalDiv[0] = domain->GlobalDivision[0];
    m_globalDiv[1] = domain->GlobalDivision[1];
    m_globalDiv[2] = domain->GlobalDivision[2];

    m_globalOffset[0] = domain->GlobalOrigin[0];
    m_globalOffset[1] = domain->GlobalOrigin[1];
    m_globalOffset[2] = domain->GlobalOrigin[2];

    m_globalRegion[0] = domain->GlobalRegion[0];
    m_globalRegion[1] = domain->GlobalRegion[1];
    m_globalRegion[2] = domain->GlobalRegion[2];

	//printf("region = %f, %f, %f\n", m_globalRegion[0], m_globalRegion[1], m_globalRegion[2]);

    int GVoxel[3] = { m_globalVoxel[0], m_globalVoxel[1], m_globalVoxel[2] }; 
    int GDiv[3]   = { m_globalDiv[0], m_globalDiv[1], m_globalDiv[2] }; 
    int head[3] = { 1, 1, 1 }; // @fixme.
    int tail[3] = { m_globalVoxel[0], m_globalVoxel[1], m_globalVoxel[2] }; // @fixme.

	delete domain;

    unsigned int step = 0; 

    if (timeSliceIndex < m_timeSteps.size()) {
       step = m_timeSteps[timeSliceIndex];
    } else {
		printf("[CDMloader] timeSliceId(%d) is out of range(max: %d)\n", timeSliceIndex, m_timeSteps.size());
		return false;
	}

    std::string dfi_filename = std::string(filename);
    
    CDM::E_CDM_ERRORCODE ret = CDM::E_CDM_SUCCESS;
    cdm_DFI* DFI_IN = cdm_DFI::ReadInit(MPI_COMM_WORLD, dfi_filename, GVoxel, GDiv, ret);
    if (ret != CDM::E_CDM_SUCCESS || DFI_IN == NULL) {
        printf("[CdmLoader] Failed to load DFI file: %s\n", filename);
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
    size_t dataSize = (GVoxel[0]+virtualCells)*(GVoxel[1]+virtualCells)*(GVoxel[2]+virtualCells);
    //printf("DBG: dataSize: %d\n", dataSize);
    float* d_v = new float[dataSize*numVariables];

    ret =  DFI_IN->ReadData(d_v,                // pointer to buffer
                            step,               // timestep
                            virtualCells/2,     // virtual cell size
                            GVoxel,             // global dim
                            GDiv,               // num divs
                            head,               // start time
                            tail,               // end time
                            r_time,             // dfi read time
                            true,               // don't read averate?
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
    m_volume.Create(GVoxel[0], GVoxel[1], GVoxel[2], numVariables, isNonUniform); // @fixme
    float* dst = m_volume.Buffer()->GetBuffer();

    size_t sx = GVoxel[0] + virtualCells;
    size_t sy = GVoxel[1] + virtualCells;
    for (size_t z = 0; z < GVoxel[2]; z++) {
        for (size_t y = 0; y < GVoxel[1]; y++) {
            for (size_t x = 0; x < GVoxel[0]; x++) {
                for (size_t c = 0; c < numVariables; c++) {
                    size_t srcIdx = (z + virtualCells/2) * sy * sx + (y + virtualCells/2) + sx + (x + virtualCells/2);
                    size_t dstIdx = z * GVoxel[1] * GVoxel[0] + y * GVoxel[0] + x;
                    float val = d_v[numVariables * srcIdx + c];
                    dst[numVariables * dstIdx + c] = val;
                }
            }
        }
    }

	if (isNonUniform) {
		std::copy(coords[0].begin(), coords[0].end(), m_volume.SpacingX()->GetBuffer());
		std::copy(coords[1].begin(), coords[1].end(), m_volume.SpacingY()->GetBuffer());
		std::copy(coords[2].begin(), coords[2].end(), m_volume.SpacingZ()->GetBuffer());
	}

    delete [] d_v;
    delete DFI_IN;

    printf("[CdmLoader] CDM load OK\n");

    return true;
}

/**
 * CDMWidth取得
 * @retval int Width
 */
int CDMLoader::Width()    {
    return m_volume.Width();
}

/**
 * CDMHeight取得
 * @retval int Height
 */
int CDMLoader::Height()   {
    return m_volume.Height();
}

/**
 * CDMDepth取得
 * @retval int Depth
 */
int CDMLoader::Depth()    {
    return m_volume.Depth();
}

/**
 * CDMComponent取得
 * @retval int Component数
 */
int CDMLoader::Component() {
    return m_volume.Component();
}

/**
 * CDMデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* CDMLoader::Buffer() {
    return m_volume.Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *CDMLoader::VolumeData()
{
    return &m_volume;
}

