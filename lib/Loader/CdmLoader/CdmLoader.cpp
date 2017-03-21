/**
 * @file CdmLoader.cpp
 * CDMデータローダー
 */
//
// TODO: - support various volume data type
//

#ifndef HIVE_WITH_CDMLIB
#error "HIVE_WITH_CDMLIB must be defined when you compile CDMLoader module"
#endif

#include <mpi.h>

#include <stdio.h>
#include <string.h>

#include <fstream>
#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-result"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#include "CdmLoader.h"
//#include "SimpleVOL.h"

#include "cdm_DFI.h"
#include "cdm_TextParser.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

class CDMLoader::Impl
{
public:
    void Clear();

    bool Load(const char *filename, int timeSliceIndex);

	int SetHeadIndex(const int headIndex[3]);
	int SetTailIndex(const int tailIndex[3]);
	int SetGlobalVoxel(const int voxelSize[3]);
	int SetGlobalDivision(const int div[3]);
	int SetDivisionMode(const int divisionMode, const int divisionAxis0 = 0,
						const int divisionAxis1 = 1);

	///
	/// The following methods are Valid after `Load`
	///

	void GlobalOffset(double ret[3])
	{
		ret[0] = m_globalOffset[0];
		ret[1] = m_globalOffset[1];
		ret[2] = m_globalOffset[2];
	}

	void GlobalRegion(double ret[3])
	{
		ret[0] = m_globalRegion[0];
		ret[1] = m_globalRegion[1];
		ret[2] = m_globalRegion[2];
	}

	void GlobalVoxel(int ret[3])
	{
		ret[0] = m_globalVoxel[0];
		ret[1] = m_globalVoxel[1];
		ret[2] = m_globalVoxel[2];
	}

	void GlobalDiv(int ret[3])
	{
		ret[0] = m_globalDiv[0];
		ret[1] = m_globalDiv[1];
		ret[2] = m_globalDiv[2];
	}

	void HeadIndex(int ret[3])
	{
		ret[0] = m_head[0];
		ret[1] = m_head[1];
		ret[2] = m_head[2];
	}

	void TailIndex(int ret[3])
	{
		ret[0] = m_tail[0];
		ret[1] = m_tail[1];
		ret[2] = m_tail[2];
	}

	int NumTimeSteps() { return m_timeSteps.size(); }

	///< Get i'th timestep
	int GetTimeStep(int i)
	{
		if (i < m_timeSteps.size())
		{
			return m_timeSteps[i];
		}
		return 0;
	}

    // Mx1 Load(each MPI rank loads all volume data: equivalent to MPI AllGather)
    bool LoadMx1(cdm_DFI *DFI_IN, unsigned int timeStep, int GVoxel[3], int GDiv[3], int virtualCells, int numVariables, bool isNonUniform, const std::vector<float> coords[3]);

    // MxM Load(each MPI rank loads corresponding data stored in .DFI)
    bool LoadMxM(cdm_DFI *DFI_IN, const cdm_Process &DFI_Process, int RankID, const std::string &infile, unsigned int timeStep, int GVoxel[3], int GDiv[3], int numVariables, bool isNonUniform, const std::vector<float> coords[3]);


	RefPtr<BufferVolumeData> m_volume;
	std::vector<unsigned int> m_timeSteps;

	double m_globalOffset[3];
	double m_globalRegion[3];
	int m_globalVoxel[3];
	int m_globalDiv[3];
	int m_head[3];
	int m_tail[3];

	int m_divisionMode;
	int m_divisionAxis0;
	int m_divisionAxis1;
};

bool CDMLoader::Impl::LoadMx1(cdm_DFI *DFI_IN, unsigned int timeStep, int GVoxel[3], int GDiv[3], int virtualCells, int numVariables, bool isNonUniform, const std::vector<float> coords[3]) {

    // Based on `fconv` tool in CDMlib package.

    float r_time;	 ///<dfiから読込んだ時間
    unsigned i_dummy; ///<平均化ステップ
    float f_dummy;	///<平均時間

    // data size
    size_t dataSize = (GVoxel[0] + virtualCells) * (GVoxel[1] + virtualCells) *
                      (GVoxel[2] + virtualCells);

    int head[3];
    int tail[3];
    
    // head/tail extent == GVoxel
    head[0] = 1;
    head[1] = 1;
    head[2] = 1;
    tail[0] = GVoxel[0];
    tail[1] = GVoxel[1];
    tail[2] = GVoxel[2];

	bool interleaved = false; // data is stored such like [XXXX...YYYY...ZZZZ]
	if ((DFI_IN->GetArrayShape() == CDM::E_CDM_NIJK))
	{
		interleaved = true; // data is stored such like [XYZYXZXYZ...]
	}
	else
	{ // must be CDM::E_CDM_IJKN
		interleaved = false;
	}
    
    // printf("DBG: dataSize: %d\n", dataSize);
    float *d_v = new float[dataSize * numVariables];

	CDM::E_CDM_ERRORCODE ret = CDM::E_CDM_SUCCESS;
    ret = DFI_IN->ReadData(d_v,				 // pointer to buffer
                           timeStep,		 // timestep
                           virtualCells / 2, // virtual cell size
                           GVoxel,			 // global dim
                           GDiv,			 // num divs
                           head,			 // head
                           tail,			 // tail
                           r_time,			 // dfi read time
                           true,			 // don't read averate?
                           i_dummy, f_dummy);

    if (ret != CDM::E_CDM_SUCCESS)
    {
        printf("[CdmLoader] Error reading CDM data.");
        delete [] d_v;
        delete DFI_IN;
        return false;
    }


    //
    // Create volume data by stripping virtual cells.
    //
    m_volume->Create(GVoxel[0], GVoxel[1], GVoxel[2], numVariables,
                     isNonUniform); // @fixme
    float *dst = m_volume->Buffer()->GetBuffer();

    // printf("virtualCells = %d\n", virtualCells);

    size_t sx = GVoxel[0] + virtualCells;
    size_t sy = GVoxel[1] + virtualCells;
    for (size_t z = 0; z < GVoxel[2]; z++)
    {
        for (size_t y = 0; y < GVoxel[1]; y++)
        {
            for (size_t x = 0; x < GVoxel[0]; x++)
            {
                for (size_t c = 0; c < numVariables; c++)
                {
                    size_t srcIdx = 0;
                    if (interleaved)
                    {
                        // size_t srcIdx =  (z + virtualCells/2) * sy * sx + (y
                        // + virtualCells/2) * sx + (x + virtualCells/2);
                        // float val = d_v[numVariables * srcIdx + c];
                        srcIdx = _CDM_IDX_NIJK(c, x, y, z, numVariables,
                                               GVoxel[0], GVoxel[1], GVoxel[2],
                                               virtualCells / 2);
                    }
                    else
                    {
                        srcIdx = _CDM_IDX_IJKN(x, y, z, c, GVoxel[0], GVoxel[1],
                                               GVoxel[2], virtualCells / 2);
                    }
                    size_t dstIdx =
                        z * GVoxel[1] * GVoxel[0] + y * GVoxel[0] + x;
                    float val = d_v[srcIdx];
                    dst[numVariables * dstIdx + c] = val;
                }
            }
        }
    }

    if (isNonUniform)
    {
        std::copy(coords[0].begin(), coords[0].end(),
                  m_volume->SpacingX()->GetBuffer());
        std::copy(coords[1].begin(), coords[1].end(),
                  m_volume->SpacingY()->GetBuffer());
        std::copy(coords[2].begin(), coords[2].end(),
                  m_volume->SpacingZ()->GetBuffer());
    }

    delete[] d_v;

    return true;
}

bool CDMLoader::Impl::LoadMxM(cdm_DFI *DFI_IN, const cdm_Process &DFI_Process, int RankID, const std::string &infile, unsigned int timeStep, int GVoxel[3], int GDiv[3], int numVariables, bool isNonUniform, const std::vector<float> coords[3]) {

    // Based on `fconv` tool in CDMlib package.
    // TODO(IDS): interleave, stepList, etc.

    int outGc = 0; // # of output guide cells. FIXME(IDS): Support outGc > 0 situation.
    int readStart[3];
    int readEnd[3];

    unsigned int avr_step; ///<平均化ステップ
    double l_dtime, avr_time;
    CDM::E_CDM_ERRORCODE ret;

    for (int i = 0; i < 3; i++) {
        readStart[i] = DFI_Process.RankList[RankID].HeadIndex[i] - outGc;
        readEnd[i] = DFI_Process.RankList[RankID].TailIndex[i] + outGc;
    }

    FILE *t_fp;
    cdm_Array* buf;
    if( (t_fp = fopen(infile.c_str(),"rb")) ) {
      fclose(t_fp);
      buf = DFI_IN->ReadFieldData(infile, timeStep, /* [out] */l_dtime,
                               readStart,
                               readEnd,
                               DFI_Process.RankList[RankID].HeadIndex,
                               DFI_Process.RankList[RankID].TailIndex,
                               /* avr_mode */true, /* [out] */avr_step, /* [out]  */avr_time, ret);
      if( ret != CDM::E_CDM_SUCCESS ) return false;

    }

    printf("buf len = %d\n", int(buf->getArrayLength()));
    // TODO(IDS): Compute offset and scale.
}

void CDMLoader::Impl::Clear()
{
	m_volume = 0;

	// Set invalid value.
	m_globalVoxel[0] = -1;
	m_globalVoxel[1] = -1;
	m_globalVoxel[2] = -1;

	m_globalDiv[0] = -1;
	m_globalDiv[1] = -1;
	m_globalDiv[2] = -1;

	m_head[0] = -1;
	m_head[1] = -1;
	m_head[2] = -1;

	m_tail[0] = -1;
	m_tail[1] = -1;
	m_tail[2] = -1;

	// use global division stored in .dfi
	m_divisionMode = DIVISION_DFI;

	m_divisionAxis0 = 0; // x
	m_divisionAxis1 = 1; // y
}

int CDMLoader::Impl::SetGlobalDivision(const int div[3])
{
	m_globalDiv[0] = div[0];
	m_globalDiv[1] = div[1];
	m_globalDiv[2] = div[2];

	return 0; // OK
}

int CDMLoader::Impl::SetGlobalVoxel(const int sz[3])
{
	if ((sz[0] <= 0) || (sz[1] <= 0) || (sz[2] <= 0))
	{
		return -1;
	}

	m_globalDiv[0] = sz[0];
	m_globalDiv[1] = sz[1];
	m_globalDiv[2] = sz[2];

	return 0; // OK
}

int CDMLoader::Impl::SetHeadIndex(const int head[3])
{
	if ((head[0] <= 0) || (head[1] <= 0) || (head[2] <= 0))
	{
		return -1;
	}

	m_head[0] = head[0];
	m_head[1] = head[1];
	m_head[2] = head[2];

	return 0; // OK
}

int CDMLoader::Impl::SetTailIndex(const int tail[3])
{
	if ((tail[0] <= 0) || (tail[1] <= 0) || (tail[2] <= 0))
	{
		return -1;
	}

	m_tail[0] = tail[0];
	m_tail[1] = tail[1];
	m_tail[2] = tail[2];

	return 0; // OK
}

int CDMLoader::Impl::SetDivisionMode(const int mode, const int axis0, const int axis1)
{
	if ((m_globalDiv[0] > 0) && (m_globalDiv[1] > 0) && (m_globalDiv[2] > 0))
	{
		// Guess `SetGlobalDivision` was called before
		return -1;
	}

	if ((mode < 0) || (mode > 4))
	{
		fprintf(stderr, "[CDMLoader] invalid division mode: %d\n", mode);
		return -1;
	}

	if (mode == 0)
	{
		m_divisionMode = DIVISION_NODIVISION;
	}
	else if (mode == 1)
	{
		m_divisionMode = DIVISION_1D;
	}
	else if (mode == 2)
	{
		m_divisionMode = DIVISION_2D;
	}
	else if (mode == 3)
	{
		m_divisionMode = DIVISION_3D;
	}
	else
	{
		assert(0);
	}

	m_divisionAxis0 = axis0;
	m_divisionAxis1 = axis1;

	return 0; // OK
}

/**
 * CDMデータのロード
 * @param filename ファイルパス
 * @param timeSliceIndex timeslice index
 * @retval true 成功
 * @retval false 失敗
 */
bool CDMLoader::Impl::Load(const char *filename, int timeSliceIndex)
{
	// Clear();
	m_volume = BufferVolumeData::CreateInstance();

	int myRank;
	int mpiSize;

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

	//
	// NOTE: Assume MPI_Init() was already called in hrender::main()
	//

	if (!std::ifstream(filename).good())
	{
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
		if (error)
		{
			fprintf(stderr, "[CDMLoader] Failed to read .dfi file: %s\n",
					tpFilename.c_str());
			return false;
		}
	}

	cdm_FileInfo fileInfo;
	{
		if (fileInfo.Read(tp) != CDM::E_CDM_SUCCESS)
		{
			fprintf(stderr,
					"[CDMLoader] Failed to read FileInfo from .dfi file: %s\n",
					tpFilename.c_str());
			return false;
		}
	}

	cdm_FilePath path;
	{
		if (path.Read(tp) != CDM::E_CDM_SUCCESS)
		{
			fprintf(stderr,
					"[CDMLoader] Failed to read FilePath from .dfi file: %s\n",
					tpFilename.c_str());
			return false;
		}
	}

	cdm_Unit unit;
	{
		if (unit.Read(tp) != CDM::E_CDM_SUCCESS)
		{
			fprintf(stderr,
					"[CDMLoader] Failed to read Unit Data from .dfi file: %s\n",
					tpFilename.c_str());
			return false;
		}
	}

	cdm_TimeSlice timeSlice;
	{
		if (timeSlice.Read(tp, fileInfo.FileFormat) != CDM::E_CDM_SUCCESS)
		{
			fprintf(stderr, "[CDMLoader] Failed to read TimeSlice Data from "
							".dfi file: %s\n",
					tpFilename.c_str());
			return false;
		}
	}

	for (size_t i = 0; i < timeSlice.SliceList.size(); i++)
	{
		// printf("timestep[%d] = %d\n", i, timeSlice.SliceList[i].step);
		m_timeSteps.push_back(timeSlice.SliceList[i].step);
	}

	//
	//  Remove current TextParser instance and swtich to read `proc.dfi`, then
	//  continue to read content of proc.dfi to get
	//  domain information.
	//
	tp.remove();

	std::string procfile = CDM::cdmPath_ConnectPath(
		dirName, CDM::cdmPath_FileName(path.ProcDFIFile, ".dfi"));
	if (!std::ifstream(procfile.c_str()).good())
	{
		fprintf(stderr, "[CDMLoader] File not found: %s\n", procfile.c_str());
		return false;
	}

	{
		int error = tp.readTPfile(procfile);
		if (error)
		{
			fprintf(stderr, "[CDMLoader] Failed to read .dfi file: %s\n",
					procfile.c_str());
			return false;
		}
	}

	bool isNonUniform = false;

	// Read Domain from proc.dfi
	cdm_Domain *domain;
	if (fileInfo.DFIType == CDM::E_CDM_DFITYPE_CARTESIAN)
	{
		domain = new cdm_Domain();
	}
	else if (fileInfo.DFIType == CDM::E_CDM_DFITYPE_NON_UNIFORM_CARTESIAN)
	{
		domain = new cdm_NonUniformDomain<double>();
		isNonUniform = true;
	}
	else
	{
		assert(0);
	}

	{
		if (domain->Read(tp, dirName) != CDM::E_CDM_SUCCESS)
		{
			fprintf(
				stderr,
				"[CDMLoader] Failed to read Doamin info from .dfi file: %s\n",
				procfile.c_str());
			return false;
		}
	}

    bool isMx1 = false; // Each MPI rank read all stored data(allgather)
	bool isMxM = false; // True if stored data division size == MPI process size
						// of hrender in running.
    bool isMxN = false; // Load M parallel data in N parallel MPI rank.

	// Load Process info.
	cdm_Process process;
	{
		if (process.Read(tp) != CDM::E_CDM_SUCCESS)
		{
			fprintf(stderr, "[CDMLoader] Warn: Failed to read Process Data "
							"from .dfi file: %s\n",
					tpFilename.c_str());
			// May OK;
		}
		else
		{

			if ((process.RankList.size() == mpiSize) &&
				(m_divisionMode == DIVISION_DFI))
			{
				// # of MPI ranks in running equals to # of MPI ranks in .DFI
				isMxM = true;
			}
		}
	}

	if (myRank == 0)
	{
		if (isMxM)
		{
			printf("Do NxN parallel loading\n");
		}

		printf("Ranks in .DFI: %d\n", int(process.RankList.size()));
		for (size_t i = 0; i < process.RankList.size(); i++)
		{
			const cdm_Rank &cdmRank = process.RankList[i];
			printf("RankList[%d] : RankID = %d, head = %d, %d, %d, tail = %d, "
				   "%d, %d\n",
				   int(i), cdmRank.RankID, cdmRank.HeadIndex[0],
				   cdmRank.HeadIndex[1], cdmRank.HeadIndex[2],
				   cdmRank.TailIndex[0], cdmRank.TailIndex[1],
				   cdmRank.TailIndex[2]);
		}
	}

	tp.remove();

	std::vector<float> coords[3];

	if (isNonUniform)
	{
		cdm_NonUniformDomain<double> *nuDomain =
			dynamic_cast<cdm_NonUniformDomain<double> *>(domain);

		coords[0].resize(nuDomain->GlobalVoxel[0] + 1);
		coords[1].resize(nuDomain->GlobalVoxel[1] + 1);
		coords[2].resize(nuDomain->GlobalVoxel[2] + 1);

		// Read coordinate data and normalize coordinate values to [0, 1]
		{
			for (size_t x = 0; x < nuDomain->GlobalVoxel[0] + 1; x++)
			{
				// printf("x[%d] = %f\n", x, nuDomain->NodeX(x));
				coords[0][x] = nuDomain->NodeX(x);
			}

			double minval = coords[0][0];
			double maxval = coords[0][nuDomain->GlobalVoxel[0]];

			for (size_t x = 0; x < nuDomain->GlobalVoxel[0] + 1; x++)
			{
				coords[0][x] = (coords[0][x] - minval) / (maxval - minval);
			}
		}

		{
			for (size_t y = 0; y < nuDomain->GlobalVoxel[1] + 1; y++)
			{
				// printf("y[%d] = %f\n", y, nuDomain->NodeY(y));
				coords[2][y] = nuDomain->NodeY(y);
			}

			double minval = coords[1][0];
			double maxval = coords[1][nuDomain->GlobalVoxel[1]];

			for (size_t x = 0; x < nuDomain->GlobalVoxel[1] + 1; x++)
			{
				coords[1][x] = (coords[1][x] - minval) / (maxval - minval);
			}
		}

		{
			for (size_t z = 0; z < nuDomain->GlobalVoxel[2] + 1; z++)
			{
				// printf("z[%d] = %f\n", z, nuDomain->NodeZ(z));
				coords[2][z] = nuDomain->NodeZ(z);
			}

			double minval = coords[2][0];
			double maxval = coords[2][nuDomain->GlobalVoxel[2]];

			for (size_t x = 0; x < nuDomain->GlobalVoxel[2] + 1; x++)
			{
				coords[2][x] = (coords[2][x] - minval) / (maxval - minval);
			}
		}
	}

	if ((m_globalVoxel[0] < 0) || (m_globalVoxel[1] < 0) ||
		(m_globalVoxel[2] < 0))
	{
		// Use global voxel size of stored data.
		m_globalVoxel[0] = domain->GlobalVoxel[0];
		m_globalVoxel[1] = domain->GlobalVoxel[1];
		m_globalVoxel[2] = domain->GlobalVoxel[2];
	}

	if (m_divisionMode == DIVISION_DFI)
	{
		m_globalDiv[0] = domain->GlobalDivision[0];
		m_globalDiv[1] = domain->GlobalDivision[1];
		m_globalDiv[2] = domain->GlobalDivision[2];
	}
	else if (m_divisionMode == DIVISION_NODIVISION)
	{
		// Gather all data for each MPI process.
		m_globalDiv[0] = 1;
		m_globalDiv[1] = 1;
		m_globalDiv[2] = 1;
	}
	else if (m_divisionMode == DIVISION_1D)
	{

		int numProcs = 1;
		MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

		m_globalDiv[0] = 1;
		m_globalDiv[1] = 1;
		m_globalDiv[2] = 1;

		m_globalDiv[m_divisionAxis0] = numProcs;
	}
	else if (m_divisionMode == DIVISION_2D)
	{

		// TODO
		assert(0);
	}
	else if (m_divisionMode == DIVISION_3D)
	{
		// TODO(IDS): Use CPMlib to find good division.
		assert(0);
		return false;
	}

	m_globalOffset[0] = domain->GlobalOrigin[0];
	m_globalOffset[1] = domain->GlobalOrigin[1];
	m_globalOffset[2] = domain->GlobalOrigin[2];

	m_globalRegion[0] = domain->GlobalRegion[0];
	m_globalRegion[1] = domain->GlobalRegion[1];
	m_globalRegion[2] = domain->GlobalRegion[2];

	// printf("region = %f, %f, %f\n", m_globalRegion[0], m_globalRegion[1],
	// m_globalRegion[2]);

	int GVoxel[3] = {-1, -1, -1};
	int GDiv[3] = {-1, -1, -1};
	int head[3] = {-1, -1, -1};
	int tail[3] = {-1, -1, -1};

	GVoxel[0] = m_globalVoxel[0];
	GVoxel[1] = m_globalVoxel[1];
	GVoxel[2] = m_globalVoxel[2];

	GDiv[0] = m_globalDiv[0];
	GDiv[1] = m_globalDiv[1];
	GDiv[2] = m_globalDiv[2];

	if (isMxM)
	{
		// TODO(IDS): Find Process info whose RankID equals to current MPI
		// rank(myRank).
		head[0] = process.RankList[myRank].HeadIndex[0];
		head[1] = process.RankList[myRank].HeadIndex[1];
		head[2] = process.RankList[myRank].HeadIndex[2];

		tail[0] = process.RankList[myRank].TailIndex[0];
		tail[1] = process.RankList[myRank].TailIndex[1];
		tail[2] = process.RankList[myRank].TailIndex[2];
	}
	else if (m_divisionMode == DIVISION_USER_SPECIFIED)
	{
		// Use user specified head/tail info.

		assert(m_head[0] > 0);
		assert(m_head[1] > 0);
		assert(m_head[2] > 0);

		assert(m_tail[0] > 0);
		assert(m_tail[1] > 0);
		assert(m_tail[2] > 0);

		head[0] = m_head[0];
		head[1] = m_head[1];
		head[2] = m_head[2];

		tail[0] = m_tail[0];
		tail[1] = m_tail[1];
		tail[2] = m_tail[2];
	}
	else
	{
		// TODO(IDS): Support Mx1, MxN loading.
		head[0] = 1;
		head[1] = 1;
		head[2] = 1;

		tail[0] = m_globalVoxel[0];
		tail[1] = m_globalVoxel[1];
		tail[2] = m_globalVoxel[2];
	}

	printf("rank[%d/%d] GVoxel: %d, %d, %d\n", myRank, mpiSize, GVoxel[0],
		   GVoxel[1], GVoxel[2]);
	printf("rank[%d/%d] GDiv: %d, %d, %d\n", myRank, mpiSize, GDiv[0], GDiv[1],
		   GDiv[2]);
	printf("rank[%d/%d] head: %d, %d, %d\n", myRank, mpiSize, head[0], head[1],
		   head[2]);
	printf("rank[%d/%d] tail: %d, %d, %d\n", myRank, mpiSize, tail[0], tail[1],
		   tail[2]);

	delete domain;

	unsigned int step = 0;

	// printf("sliceIndex= %d\n", timeSliceIndex);
	if (timeSliceIndex < m_timeSteps.size())
	{
		step = m_timeSteps[timeSliceIndex];
	}
	else
	{
		printf("[CDMloader] timeSliceId(%d) is out of range(max: %d)\n",
			   timeSliceIndex, int(m_timeSteps.size()));
		return false;
	}

	std::string dfi_filename = std::string(filename);

	CDM::E_CDM_ERRORCODE ret = CDM::E_CDM_SUCCESS;
	cdm_DFI *DFI_IN =
		cdm_DFI::ReadInit(MPI_COMM_WORLD, dfi_filename, GVoxel, GDiv, ret);
	if (ret != CDM::E_CDM_SUCCESS || DFI_IN == NULL)
	{
		printf("[CdmLoader] Failed to load DFI file: %s\n", filename);
		return false;
	}

	if ((DFI_IN->GetArrayShape() == CDM::E_CDM_NIJK) ||
		(DFI_IN->GetArrayShape() == CDM::E_CDM_IJKN))
	{
		// ok
	}
	else
	{
		printf("[CdmLoader] Unsupported array shape.\n");
		return false;
	}

	bool interleaved = false; // data is stored such like [XXXX...YYYY...ZZZZ]
	if ((DFI_IN->GetArrayShape() == CDM::E_CDM_NIJK))
	{
		interleaved = true; // data is stored such like [XYZYXZXYZ...]
	}
	else
	{ // must be CDM::E_CDM_IJKN
		interleaved = false;
	}

	// Check data type
	// @todo { Support more data type. }
	// if( (DFI_IN->GetDataType() == CDM::E_CDM_UINT8) ||
	//    (DFI_IN->GetDataType() == CDM::E_CDM_FLOAT32) ||
	//    (DFI_IN->GetDataType() == CDM::E_CDM_FLOAT64) ) {
	if (DFI_IN->GetDataType() == CDM::E_CDM_FLOAT32)
	{
		// OK
	}
	else
	{
		printf("[CdmLoader] Unsupported data type: %s\n",
			   DFI_IN->GetDataTypeString().c_str());
		return CDM::E_CDM_ERROR;
	}

	int numVariables = DFI_IN->GetNumVariables();
	//
	int numGuideCells = DFI_IN->GetNumGuideCell();
	printf("[CdmLoader] NumGuideCells = %d\n", numGuideCells);

	int virtualCells = 2 * numGuideCells;

	// Get unit
	std::string Lunit;
	double Lref, Ldiff;
	bool LBset;
	ret = DFI_IN->GetUnit("Length", Lunit, Lref, Ldiff, LBset);
	if (ret == CDM::E_CDM_SUCCESS)
	{
		printf("Length\n");
		printf("  Unit      : %s\n", Lunit.c_str());
		printf("  reference : %e\n", Lref);
		if (LBset)
		{
			printf("  difference: %e\n", Ldiff);
		}
	}

    if (isMx1) {

#if 0
        float r_time;	 ///<dfiから読込んだ時間
        unsigned i_dummy; ///<平均化ステップ
        float f_dummy;	///<平均時間

        // data size
        size_t dataSize = (GVoxel[0] + virtualCells) * (GVoxel[1] + virtualCells) *
                          (GVoxel[2] + virtualCells);
        // printf("DBG: dataSize: %d\n", dataSize);
        float *d_v = new float[dataSize * numVariables];

        ret = DFI_IN->ReadData(d_v,				 // pointer to buffer
                               step,			 // timestep
                               virtualCells / 2, // virtual cell size
                               GVoxel,			 // global dim
                               GDiv,			 // num divs
                               head,			 // start time
                               tail,			 // end time
                               r_time,			 // dfi read time
                               true,			 // don't read averate?
                               i_dummy, f_dummy);

        if (ret != CDM::E_CDM_SUCCESS)
        {
            printf("[CdmLoader] Error reading CDM data.");
            // delete [] d_v;
            delete DFI_IN;
            return false;
        }


        //
        // Create volume data by stripping virtual cells.
        //
        m_volume->Create(GVoxel[0], GVoxel[1], GVoxel[2], numVariables,
                         isNonUniform); // @fixme
        float *dst = m_volume->Buffer()->GetBuffer();

        // printf("virtualCells = %d\n", virtualCells);

        size_t sx = GVoxel[0] + virtualCells;
        size_t sy = GVoxel[1] + virtualCells;
        for (size_t z = 0; z < GVoxel[2]; z++)
        {
            for (size_t y = 0; y < GVoxel[1]; y++)
            {
                for (size_t x = 0; x < GVoxel[0]; x++)
                {
                    for (size_t c = 0; c < numVariables; c++)
                    {
                        size_t srcIdx = 0;
                        if (interleaved)
                        {
                            // size_t srcIdx =  (z + virtualCells/2) * sy * sx + (y
                            // + virtualCells/2) * sx + (x + virtualCells/2);
                            // float val = d_v[numVariables * srcIdx + c];
                            srcIdx = _CDM_IDX_NIJK(c, x, y, z, numVariables,
                                                   GVoxel[0], GVoxel[1], GVoxel[2],
                                                   virtualCells / 2);
                        }
                        else
                        {
                            srcIdx = _CDM_IDX_IJKN(x, y, z, c, GVoxel[0], GVoxel[1],
                                                   GVoxel[2], virtualCells / 2);
                        }
                        size_t dstIdx =
                            z * GVoxel[1] * GVoxel[0] + y * GVoxel[0] + x;
                        float val = d_v[srcIdx];
                        dst[numVariables * dstIdx + c] = val;
                    }
                }
            }
        }

        if (isNonUniform)
        {
            std::copy(coords[0].begin(), coords[0].end(),
                      m_volume->SpacingX()->GetBuffer());
            std::copy(coords[1].begin(), coords[1].end(),
                      m_volume->SpacingY()->GetBuffer());
            std::copy(coords[2].begin(), coords[2].end(),
                      m_volume->SpacingZ()->GetBuffer());
        }

	    delete[] d_v;
#else
        bool ok = LoadMx1(DFI_IN, step, GVoxel, GDiv, virtualCells, numVariables, isNonUniform, coords);

        if (!ok) {
            return false;
        }
#endif

    } else if (isMxM) {

        // bool CDMLoader::Impl::LoadMxM(cdm_DFI *DFI_IN, const cdm_Process &DFI_Process, int RankID, const std::string &infile, unsigned int timeStep, int GVoxel[3], int GDiv[3], int numVariables, bool isNonUniform, const std::vector<float> coords[3]) {

        int RankID = myRank;

        unsigned int l_step = step; // TODO(IDS): Use SliceList.step

        bool mio = false;
        if (DFI_IN->GetcdmMPI()->NumberOfRank > 1) mio = true;

        //読込みファイル名の生成
        std::string inPath = CDM::cdmPath_DirName(DFI_IN->get_dfi_fname());
        std::string infile =  CDM::cdmPath_ConnectPath(inPath, DFI_IN->Generate_FieldFileName(process.RankList[RankID].RankID,l_step,mio));


        bool ok = LoadMxM(DFI_IN, process, RankID, infile, step, GVoxel, GDiv, numVariables, isNonUniform, coords);

        if (!ok) {
            return false;
        }

    } else {
        // TODO
        assert(0);
    }

	delete DFI_IN;

	printf("[CdmLoader] CDM load OK\n");

	return true;
}


// ------------------------------------------------------


/// コンストラクタ
CDMLoader::CDMLoader() : m_imp(new CDMLoader::Impl()) { Clear(); }

/// デストラクタ
CDMLoader::~CDMLoader() {
    Clear();

    delete m_imp;
}

/// ボリュームクリア
void CDMLoader::Clear()
{
    m_imp->Clear();
}

/**
 * CDMデータのロード
 * @param filename ファイルパス
 * @param timeSliceIndex timeslice index
 * @retval true 成功
 * @retval false 失敗
 */
bool CDMLoader::Load(const char *filename, int timeSliceIndex)
{
    return m_imp->Load(filename, timeSliceIndex);
}

/**
 * CDMWidth取得
 * @retval int Width
 */
int CDMLoader::Width() { assert(m_imp->m_volume); return m_imp->m_volume->Width(); }

/**
 * CDMHeight取得
 * @retval int Height
 */
int CDMLoader::Height() { assert(m_imp->m_volume); return m_imp->m_volume->Height(); }

/**
 * CDMDepth取得
 * @retval int Depth
 */
int CDMLoader::Depth() { assert(m_imp->m_volume); return m_imp->m_volume->Depth(); }

/**
 * CDMComponent取得
 * @retval int Component数
 */
int CDMLoader::Component() { assert(m_imp->m_volume); return m_imp->m_volume->Component(); }

/**
 * CDMデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer *CDMLoader::Buffer() { assert(m_imp->m_volume); return m_imp->m_volume->Buffer(); }

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *CDMLoader::VolumeData() { return m_imp->m_volume; }

void CDMLoader::GlobalOffset(double ret[3])
{
    m_imp->GlobalOffset(ret);
}

void CDMLoader::GlobalRegion(double ret[3])
{
    m_imp->GlobalRegion(ret);
}

void CDMLoader::GlobalVoxel(int ret[3])
{
    m_imp->GlobalVoxel(ret);
}

void CDMLoader::GlobalDiv(int ret[3])
{
    m_imp->GlobalDiv(ret);
}

void CDMLoader::HeadIndex(int ret[3])
{
    m_imp->HeadIndex(ret);
}

void CDMLoader::TailIndex(int ret[3])
{
    m_imp->TailIndex(ret);
}

int CDMLoader::NumTimeSteps()
{
    return m_imp->NumTimeSteps();
}

int CDMLoader::GetTimeStep(int i)
{
    return m_imp->GetTimeStep(i);
}

int CDMLoader::SetHeadIndex(const int headIndex[3]) {
    m_imp->SetHeadIndex(headIndex);
}

int CDMLoader::SetTailIndex(const int tailIndex[3]) {
    m_imp->SetTailIndex(tailIndex);
}

int CDMLoader::SetGlobalVoxel(const int voxelSize[3]) {
    m_imp->SetGlobalVoxel(voxelSize);
}

int CDMLoader::SetGlobalDivision(const int div[3]) {
    m_imp->SetGlobalDivision(div);
}

int CDMLoader::SetDivisionMode(const int divisionMode, const int divisionAxis0, const int divisionAxis1) {
    m_imp->SetDivisionMode(divisionMode, divisionAxis0, divisionAxis1);
}
