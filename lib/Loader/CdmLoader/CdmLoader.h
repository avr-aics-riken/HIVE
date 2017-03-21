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
		DIVISION_DFI = 4,
		DIVISION_USER_SPECIFIED = 5
	};

	CDMLoader();
	~CDMLoader();
	void Clear();

	/// Manually specify Head index.
	/// Must be called before `Load()`.
	/// Without calling this function, CDMLoader uses the setting of Head index
	/// according to
	/// the divisionMode.
	/// Head may have different values for each MPI rank to achieve
	/// data-parallel loading.
	/// Returns 0 upon success.
	/// Returns <0 upon error.
	int SetHeadIndex(const int headIndex[3]);

	/// Manually specify Tail index.
	/// Must be called before `Load()`.
	/// Without calling this function, CDMLoader uses the setting of Tail index
	/// according to
	/// the divisionMode.
	/// Tail may have different values for each MPI rank to achieve
	/// data-parallel loading.
	/// Returns 0 upon success.
	/// Returns <0 upon error.
	int SetTailIndex(const int tailIndex[3]);

	/// Manually specify global voxel size.
	/// The value must be all same for each MPI rank.
	/// Must be called before `Load()`.
	/// Without calling this function, CDMLoader uses global voxel size in the
	/// file which is determined at `Load` function.
	/// Returns 0 upon success.
	/// Returns <0 upon error.
	int SetGlobalVoxel(const int voxelSize[3]);

	/// Manually specify global division
	/// The value must be all same for each MPI rank.
	/// `SetGlobalDivision` wins when `SetDivisionMode` was also called.
	/// Returns 0 upon success.
	/// Returns <0 upon error.
	int SetGlobalDivision(const int div[3]);

	/// Set division mode and automatically compute global division internally.
	/// The value must be all same for each MPI rank.
	/// `divisionMode` specifies the mode for data prallel loading.
	///   0: No data parallel loading(Mx1 loading)
	///   1: Data parallel loading with 1D division(MxN loading. split at
	///   `divisionAxis0` axis)
	///   2: Data parallel loading with 2D division(MxN loading. split at
	///   `divisionAxis0` and `divisionAxis1` axis)
	///   3: Data parallel loading with 3D division(MxN loading). Automatically
	///   calculate division.
	///   4: Use global division store in DFI(NxN loading).
	///   5: Use user supplied value(Head, Tail, GlobalDivision must be
	///   specified manually)
	/// `divisionAxis0` : Axis for 1D and 2D division(0: x, 1:y, 2:z)
	/// `divisionAxis1` : Axis for 2D division(0: x, 1:y, 2:z)
	///
	/// Returns 0 upon success.
	/// Returns <0 upon error or `SetGlobalDivision` was called before.
	int SetDivisionMode(const int divisionMode, const int divisionAxis0 = 0,
						const int divisionAxis1 = 1);

	/// Load CDMlib data.
	/// timeSliceIndex may have different value for each MPI rank.
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

	void GlobalOffset(double ret[3]);
	//{
	//	ret[0] = m_globalOffset[0];
	//	ret[1] = m_globalOffset[1];
	//	ret[2] = m_globalOffset[2];
	//}

	void GlobalRegion(double ret[3]);
	//{
	//	ret[0] = m_globalRegion[0];
	//	ret[1] = m_globalRegion[1];
	//	ret[2] = m_globalRegion[2];
	//}

	void GlobalVoxel(int ret[3]);
	//{
	//	ret[0] = m_globalVoxel[0];
	//	ret[1] = m_globalVoxel[1];
	//	ret[2] = m_globalVoxel[2];
	//}

	void GlobalDiv(int ret[3]);
	//{
	//	ret[0] = m_globalDiv[0];
	//	ret[1] = m_globalDiv[1];
	//	ret[2] = m_globalDiv[2];
	//}

	void HeadIndex(int ret[3]);
	//{
	//	ret[0] = m_head[0];
	//	ret[1] = m_head[1];
	//	ret[2] = m_head[2];
	//}

	void TailIndex(int ret[3]);
	//{
	//	ret[0] = m_tail[0];
	//	ret[1] = m_tail[1];
	//	ret[2] = m_tail[2];
	//}

	int NumTimeSteps(); // { return m_timeSteps.size(); }

	///< Get i'th timestep
	int GetTimeStep(int i); 
	//{
	//	if (i < m_timeSteps.size())
	//	{
	//		return m_timeSteps[i];
	//	}
	//	return 0;
	//}

	FloatBuffer *Buffer();

	BufferVolumeData *VolumeData();

  private:
  class Impl;
  Impl* m_imp;

	//RefPtr<BufferVolumeData> m_volume;
	//std::vector<unsigned int> m_timeSteps;

	//double m_globalOffset[3];
	//double m_globalRegion[3];
	//int m_globalVoxel[3];
	//int m_globalDiv[3];
	//int m_head[3];
	//int m_tail[3];

	//int m_divisionMode;
	//int m_divisionAxis0;
	//int m_divisionAxis1;
};

#endif //_CDMLOADER_H_
