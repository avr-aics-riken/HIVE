/**
 * @file PdbLoader.h
 * PDBデータローダー
 */
#ifndef _PDBLOADERNATIVE_H
#define _PDBLOADERNATIVE_H

#include "Ref.h"
#include "BufferPointData.h"
#include "BufferLineData.h"

#include "tiny_pdb.h"

#include <vector>
#include <string>

/**
 * PDBデータローダー
 */
class PDBLoader : public RefCount
{
public:
	PDBLoader();
	~PDBLoader();
	void Clear();
	BufferPointData ball;
	BufferLineData stick;
protected:
	bool Load(const char* filename, bool generateBond = false);
	BufferPointData* BallData();
	BufferLineData* StickData();

	int NumAtoms() const {
		return static_cast<int>(m_atoms.size());
	}

	/// Set material ID for i'th atom.
	void SetMaterial(int i, float matID);

   	/// Element symbol name of i'th atom.
	std::string AtomElementSymbol(int i);

private:
	std::vector<tinypdb::Atom> m_atoms;
};

#endif //_PDBLOADERNATIVE_H

