/**
 * @file PdmLoader.h
 * PDMデータローダー
 */
#ifndef _PDMLOADERNATIVE_H
#define _PDMLOADERNATIVE_H

#include <cstdlib>

#include "Ref.h"
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
	BufferPointData points;
protected:
	bool Load(const char* filename, int timeStep = -1);
	BufferPointData* PointData();
};

#endif //_PDMLOADERNATIVE_H

