/**
 * @file CdmLoader.h
 * CDMデータローダー
 */
#ifndef _CDMLOADER_H_
#define _CDMLOADER_H_

#include "Buffer.h"
#include "BufferVolumeData.h"
#include "Ref.h"

//
// GlobalOffset
// |
// |/
//
// +-------------------------------+
// |                               |
// |                               |
// |                               |
// |            LocalOffset        |
// |              |                |
// |              |/               |
// |                               |
// |              +================+
// |              |                |
// |              |                |
// |              |                |
// |              |                |
// |              |                |
// +--------------+================+
//                <- LocalRegion  ->
//
// <------  GlobalRegion     ------>
//
//
// Global : Whole volume region
// Local  : Actual volume region per MPI ranks.
//          For Mx1 loading. Global == Local
//
// Note that LocalOffset is an absolute value and within
//
// [GlobalOffset, GlobalOffset + GlobalRegion]
//

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

    enum LOAD_MODE
    {
        LOAD_Mx1 = 0, // Each MPI rank read all stored data(allgather)
        LOAD_MxM = 1, // Stored data division size == MPI process size
                      // of hrender in running.
        LOAD_MxN = 2  // Load M parallel data in N parallel MPI rank.
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
    bool LoadMx1(const char *filename, int timeSliceIndex = 0);
    bool LoadMxM(const char *filename, int timeSliceIndex = 0);
    bool LoadMxN(const char *filename, int divX, int divY, int divZ, int timeSliceIndex = 0);

	///
	/// The following methods are Valid after `Load`
	///

	int Width();	 ///< Width of local voxel
	int Height();	///< Height of local voxel
	int Depth();	 ///< Depth of local voxel
	int Component(); ///< The number of components

	/// Global volume offset(same for all MPI ranks)
	void GlobalOffset(double ret[3]);

	/// Global region(same for all MPI ranks)
	void GlobalRegion(double ret[3]);

	/// Global voxel size(same for all MPI ranks)
	void GlobalVoxel(int ret[3]);

	/// Global division(same for all MPI ranks)
	void GlobalDiv(int ret[3]);

	/// Local volume offset(offset to actual volume data. may have different
	/// value for each MPI rank)
	void LocalOffset(double ret[3]);

	/// Local volume region(region of actual volume data. may have different
	/// value for each MPI rank)
	void LocalRegion(double ret[3]);

	/// Local voxel size(voxel size of actual volume data. may have different
	/// value for each MPI rank)
	void LocalVoxel(int ret[3]);

	void HeadIndex(int ret[3]);
	void TailIndex(int ret[3]);

	int NumTimeSteps(); // { return m_timeSteps.size(); }

	///< Get i'th timestep
	int GetTimeStep(int i);

	FloatBuffer *Buffer();

	BufferVolumeData *VolumeData();

  private:
	class Impl;
	Impl *m_imp;
};

#endif //_CDMLOADER_H_
