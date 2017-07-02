/**
 * @file PdmLoader.h
 * PDMデータローダー
 */
#ifndef _PDMLOADERNATIVE_H
#define _PDMLOADERNATIVE_H

#include <cstdlib>
#include <map>
#include <string>

#include "Buffer.h"
#include "BufferExtraData.h"
#include "BufferPointData.h"
#include "Ref.h"

/**
 * PDMデータローダー
 */
class PDMLoader : public RefCount
{
  public:
	enum Type
	{
		PDM_TYPE_INVALID,
		PDM_TYPE_INT,
		PDM_TYPE_FLOAT,
		PDM_TYPE_DOUBLE
	};

	typedef struct
	{
		std::string name;
		std::string type_name;
		Type type;
		int n; // # of components

		// Pointer to data(filled when `PdmLib::ReadAll` was called.
		union {
			int *i_ptr;
			float *f_ptr;
			double *d_ptr;
		} data_ptr;

    size_t num_data;  // # of data read.

	} ContainerInfo;

	PDMLoader();
	~PDMLoader();
	void Clear();

  protected:
	/**
	 * Enable PDMLib's profiling functionality.
	 * Must be called before `Load` method.
	 * Returns false when this method was called after `Load` function.
	 */
	bool EnableProfiling(bool onoff);

	/**
	 * Loads PDM data for a given timeStep(-1 = read the first timestep in the
   * PDM file).
   * `coordinateName` specifies the name of coordinate container.
   * Set NULL to disable reading coordinate container.
   * When `migration` is set to true, do MxN loading of data(Load data in data
   * parallel manner).
   * `migration` will be ignored when `coordinateName` is set to NULL.
	 */
	bool Load(const char *filename, int timeStep = -1,
			  const char *coordinateName = "Coordinate",
			  bool migration = false);

	/**
	 * Mx1 loading version of `Load`.
   * Each MPI ranks loads all PDM data.
	 */
  bool LoadMx1(const char *filename, int timeStep = 1, const char *coordinateName = "Coordinate");

	/**
	 * Read point(coordinate) data.
	 */
	BufferPointData *PointData(const char *containerName = "Coordinate",
							   float radius = 1.0);

	/**
	 * Read cutom attribute(container).
	 */
	BufferExtraData *ExtraData(const char *containerName);

	bool m_initialized; // True upon the success of `Load`.
	bool m_profiling;
	std::string m_coordianteName;

	// Store coordinate attribute(vec3 pos + radius).
	std::map<std::string, RefPtr<BufferPointData> > m_pointMap;

	// Store custom attributes(containers)
	std::map<std::string, RefPtr<BufferExtraData> > m_containerMap;

	std::vector<ContainerInfo> m_containerInfoList;

  private:
	bool CachePoints(const char *containerName);
	bool CacheExtraData(const char *containerName);
};

#endif //_PDMLOADERNATIVE_H
