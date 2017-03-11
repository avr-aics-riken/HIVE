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

	} ContainerInfo;

	PDMLoader();
	~PDMLoader();
	void Clear();

  protected:
	/**
	 * Loads PDM data for a given timeStep(-1 = read the first timestep in the
   * PDM file).
   * When `migration` is set to true, do MxN loading of data(Load data in data
   * parallel manner).
	 */
	bool Load(const char *filename, int timeStep = -1, bool migration = false);

	/**
	 * Read point(coordinate) data.
	 */
	BufferPointData *PointData(const char *containerName = "Coordinate",
							   float radius = 1.0);

	/**
	 * Read cutom attribute(container).
	 */
	BufferExtraData *ExtraData(const char *containerName);

	int m_timeStep;
	bool m_readAll;
	bool m_migration;
	bool m_ok;		  // True upon the success of `Load`.
	size_t m_numData; // data size read.

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
