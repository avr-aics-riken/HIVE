/**
 * @file PdmLoader.h
 * PDMデータローダー
 */
#ifndef _PDMLOADERNATIVE_H
#define _PDMLOADERNATIVE_H

#include <cstdlib>
#include <map>
#include <string>

#include "Ref.h"
#include "Buffer.h"
#include "BufferPointData.h"
#include "BufferExtraData.h"

/**
 * PDMデータローダー
 */
class PDMLoader : public RefCount
{
public:
	typedef struct {
		std::string name;
		std::string type;
	} ContainerInfo;

	PDMLoader();
	~PDMLoader();
	void Clear();

protected:

	/**
	 * Loads PDM data for a given timeStep(-1 = read the first timestep in the PDM file).
	 */
	bool Load(const char* filename, int timeStep = -1);

	/**
	 * Read point(coordinate) data.
	 */
	BufferPointData* PointData(const char* containerName = "Coordinate", float radius = 1.0);

	/**
	 * Read cutom attribute(container).
	 */
	BufferExtraData* ExtraData(const char* containerName);

	int m_timeStep;
	bool m_readAll;

	// Store coordinate attribute.
	std::map<std::string, RefPtr<BufferPointData> > m_pointMap;

	// Store custom attributes(containers)
	std::map<std::string, RefPtr<BufferExtraData> > m_containerMap;

	std::vector<ContainerInfo> m_containerInfoList;

private:
	bool CachePoints(const char* containerName, int timeStep);
	bool CacheExtraData(const char* containerName, int timeStep);
};

#endif //_PDMLOADERNATIVE_H
