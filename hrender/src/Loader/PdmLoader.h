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

// @todo { Add interface to load custom(meta) data from PDMlib }

/**
 * PDMデータローダー
 */
class PDMLoader : public RefCount
{
public:
	PDMLoader();
	~PDMLoader();
	void Clear();

protected:
	bool Load(const char* filename, int timeStep = -1);
	BufferPointData* PointData(const char* containerName = "Coordinate", float radius = 1.0);
	Vec3Buffer* Vec3Data(const char* containerName);
	FloatBuffer* FloatData(const char* containerName);
	UintBuffer* UintData(const char* containerName);

	int m_timeStep;
	bool m_readAll;

	std::map<std::string, RefPtr<BufferPointData> > m_pointMap;
	std::map<std::string, RefPtr<Vec3Buffer> > m_vec3Map;
	std::map<std::string, RefPtr<FloatBuffer> > m_floatMap;
	std::map<std::string, RefPtr<UintBuffer> > m_uintMap;

};

#endif //_PDMLOADERNATIVE_H

