#ifndef _PDMLOADERNATIVE_H
#define _PDMLOADERNATIVE_H

#include "Ref.h"
#include "BufferPointData.h"

// @todo { Add interface to load custom(meta) data from PDMlib }

class PDMLoader : public RefCount
{
public:
	PDMLoader();
	~PDMLoader();
	void Clear();
	BufferPointData points;
protected:
	bool Load(const char* filename);
	BufferPointData* PointData();
};

#endif //_PDMLOADERNATIVE_H

