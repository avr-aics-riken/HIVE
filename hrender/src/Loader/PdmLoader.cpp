#include "PdmLoader.h"

#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile CDMLoader module"
#endif

#include <cstring>

#include "PDMlib.h"

PDMLoader::PDMLoader(){}
PDMLoader::~PDMLoader(){};

void PDMLoader::Clear()
{
	points.Clear();
}

bool PDMLoader::Load(const char* filename){
	Clear();

	// @todo {}
	return false;
}

BufferPointData *PDMLoader::PointData()
{
	return &points;
}
