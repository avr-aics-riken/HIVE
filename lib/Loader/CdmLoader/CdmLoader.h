/**
 * @file CdmLoader.h
 * CDMデータローダー
 */
#ifndef _CDMLOADER_H_
#define _CDMLOADER_H_

#include "Buffer.h"
#include "BufferVolumeData.h"
#include "Ref.h"

/**
 * CDMデータローダー
 */
class CDMLoader : public RefCount
{
  public:
	enum DIVISION_MODE
	{
		DIVISION_NODIVISION = 0,
		DIVISION_1D = 1,
		DIVISION_2D = 2,
		DIVISION_3D = 3,
		DIVISION_DFI = 4
	};

	CDMLoader();
	~CDMLoader();
	void Clear();

	/// Manually specify global voxel size.
	/// Must be called before `Load()`.
	/// Without calling this function, CDMLoader uses global voxel size in the
	/// file which is determined at `Load` function.
	/// Returns 0 upon success.
	/// Returns <0 upon error.
	int SetGlobalVoxelSize(const int voxelSize[3]);

	/// Manually specify global division
	/// `SetGlobalDivision` wins when `SetDivisionMode` was also called.
	/// Returns 0 upon success.
	/// Returns <0 upon error.
	int SetGlobalDivision(const int div[3]);

	/// Set division mode and automatically compute global division internally.
	/// `divisionMode` specifies the mode for data prallel loading.
	///   0: No data parallel loading(Mx1 loading)
	///   1: Data parallel loading with 1D division(MxN loading. split at
	///   `divisionAxis0` axis)
	///   2: Data parallel loading with 2D division(MxN loading. split at
	///   `divisionAxis0` and `divisionAxis1` axis)
	///   3: Data parallel loading with 3D division(MxN loading). Automatically
	///   calculate division.
	///   4: Use global division store in DFI(CDMlib file).
	/// `divisionAxis0` : Axis for 1D and 2D division(0: x, 1:y, 2:z)
	/// `divisionAxis1` : Axis for 2D division(0: x, 1:y, 2:z)
	///
	/// Returns 0 upon success.
	/// Returns <0 upon error or `SetGlobalDivision` was called before.
	int SetDivisionMode(const int divisionMode, const int divisionAxis0 = 0,
						const int divisionAxis1 = 1);

	/// Load CDMlib data.
	/// To enable data parallel loading, `SetGlobalDivision` or
	/// `SetDivisionMode` must be called before this `Load` function.
	bool Load(const char *filename, int timeSliceIndex = 0);

	///
	/// The following methods are Valid after `Load`
	///

	int Width();
	int Height();
	int Depth();
	int Component();

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

	FloatBuffer *Buffer();

	BufferVolumeData *VolumeData();

  private:
	RefPtr<BufferVolumeData> m_volume;
	std::vector<unsigned int> m_timeSteps;

	double m_globalOffset[3];
	double m_globalRegion[3];
	int m_globalVoxel[3];
	int m_globalDiv[3];

	int m_divisionMode;
	int m_divisionAxis0;
	int m_divisionAxis1;
};

#endif //_CDMLOADER_H_
