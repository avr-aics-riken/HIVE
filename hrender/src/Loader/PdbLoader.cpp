#include "PdbLoader.h"
#include "tiny_pdb.h"

#include <cstring>

PDBLoader::PDBLoader(){}
PDBLoader::~PDBLoader(){};

void PDBLoader::Clear()
{
	ball.Clear();
}

bool PDBLoader::Load(const char* filename){
	Clear();

    tinypdb::TinyPDB pdb(filename);
    if (pdb.Parse(/* isBondGenerated = */ true)) {
        printf("[PDBLoader] PDB parsing failed: %s \n", filename);
        return false;
    }

    const size_t numAtoms = pdb.GetAtoms().size();

    ball.Create(numAtoms);
    Vec3Buffer*  pos     = ball.Position();
    FloatBuffer* mat     = ball.Material();
    FloatBuffer* radius  = ball.Radius();

	printf("[PDBLoader] # of atoms: %ld\n", numAtoms);

    float* pp = pos->GetBuffer();
	for (size_t i = 0; i < numAtoms; i++) {
		pp[3*i+0] = pdb.GetAtoms()[i].GetX();
		pp[3*i+1] = pdb.GetAtoms()[i].GetY();
		pp[3*i+2] = pdb.GetAtoms()[i].GetZ();
	}

	// @todo
    float* rad = radius->GetBuffer();
    for (int i = 0; i < numAtoms; ++i) {
        rad[i] = 1.0f;
    }

	// @todo
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());

	return true;
}

BufferPointData *PDBLoader::BallData()
{
	return &ball;
}
