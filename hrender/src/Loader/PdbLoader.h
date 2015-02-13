#ifndef _PDBLOADERNATIVE_H
#define _PDBLOADERNATIVE_H

#include "Ref.h"
#include "BufferPointData.h"
#include "BufferLineData.h"

class PDBLoader : public RefCount
{
public:
	PDBLoader();
	~PDBLoader();
	void Clear();
	BufferPointData ball;
	BufferLineData stick;
protected:
	bool Load(const char* filename);
	BufferPointData* BallData();
	BufferLineData* StickData();
};

#endif //_PDBLOADERNATIVE_H

