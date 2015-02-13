#ifndef _PDBLOADERNATIVE_H
#define _PDBLOADERNATIVE_H

#include "Ref.h"
#include "BufferPointData.h"

class PDBLoader : public RefCount
{
public:
	PDBLoader();
	~PDBLoader();
	void Clear();
	BufferPointData ball;
  // @todo { stick as line primitive. }
protected:
	bool Load(const char* filename);
	BufferPointData* BallData();
};

#endif //_PDBLOADERNATIVE_H

