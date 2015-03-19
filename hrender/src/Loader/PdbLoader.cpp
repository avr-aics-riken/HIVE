/**
 * @file PdbLoader.cpp
 * PDBデータローダー
 */
#include "PdbLoader.h"
#include "tiny_pdb.h"
#include "Buffer.h"
#include <cstring>

/// コンストラクタ
PDBLoader::PDBLoader(){}

/// デストラクタ
PDBLoader::~PDBLoader(){};

/// メンバクリア
void PDBLoader::Clear()
{
	ball.Clear();
	stick.Clear();
	m_atoms.clear();
}

/**
 * BCMデータのロード
 * @param filename ファイルパス
 * @param generateBond 線primitiveとの接点作成
 * @retval true 成功
 * @retval false 失敗
 */
bool PDBLoader::Load(const char* filename, bool generateBond){
	Clear();

    tinypdb::TinyPDB pdb(filename);
    if (pdb.Parse(/* isBondGenerated = */ generateBond)) {
        fprintf(stderr,"[PDBLoader] PDB parsing failed: %s \n", filename);
        return false;
    }

	m_atoms = pdb.GetAtoms(); // copy

    int numAtoms = static_cast<int>(pdb.GetAtoms().size());

    {
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

        // @fixme
        float* rad = radius->GetBuffer();
        for (int i = 0; i < numAtoms; ++i) {
            rad[i] = 0.25f;
        }

        // @todo
        memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());
    }


    if (generateBond) {

        // We reprent Bond as line primitives.
        std::vector<float> bondLines;
        for (unsigned int i = 0; i < numAtoms; i++) {

          tinypdb::Atom& atom = pdb.GetAtoms()[i];

          for (unsigned int j = 0; j < atom.GetBonds().size(); j++) {
            const tinypdb::Atom* dst = atom.GetBonds()[j];

            if (dst->Visited()) {
              continue;
            }

            bondLines.push_back(atom.GetX());
            bondLines.push_back(atom.GetY());
            bondLines.push_back(atom.GetZ());

            bondLines.push_back(dst->GetX());
            bondLines.push_back(dst->GetY());
            bondLines.push_back(dst->GetZ());

          }

          atom.SetVisited(true); 

        }

        size_t numBonds = bondLines.size() / 3 / 2;
        size_t numBondVertices = numBonds * 2;

        printf("[PDBLoader] # of bonds: %ld\n", numBonds);

        stick.Create(numBondVertices, /* index num = */0, /* use vrying radius */false);
        Vec3Buffer*  pos     = stick.Position();
        FloatBuffer* mat     = stick.Material();
        //FloatBuffer* radius  = stick.Radius();
        //UintBuffer*  index   = stick.Index();  // not used.

        float* pp = pos->GetBuffer();
        for (size_t i = 0; i < numBondVertices; i++) {
            pp[3*i+0] = bondLines[3*i+0];
            pp[3*i+1] = bondLines[3*i+1];
            pp[3*i+2] = bondLines[3*i+2];
        }

		// Don't use varying radius.
		//float* rad = radius->GetBuffer();
		//for (int i = 0; i < numBondVertices; ++i) {
		//	rad[i] = 1.0f;
		//}

		// @todo
		memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());

	}

	return true;
}

/**
 * 点データ取得
 * @retval BufferPointData* 点データバッファへの参照
 */
BufferPointData *PDBLoader::BallData()
{
	return &ball;
}

/**
 * 線データ取得
 * @retval BufferLineData* Lineデータバッファへの参照
 */
BufferLineData *PDBLoader::StickData()
{
	return &stick;
}


/**
 * マテリアル設定
 * @param i 対象index
 * @param matID マテリアルID
 */
void PDBLoader::SetMaterial(int i, float matID) {
    FloatBuffer* mat     = ball.Material();
	if (i < 0 || i >= mat->GetNum()) {
		return;
	}

	mat->GetBuffer()[i] = matID;
}

/**
 * AtomElementSymbol取得
 * @param i 対象index
 * @retval GetElementSymbol 取得した要素[std::string]
 */
std::string PDBLoader::AtomElementSymbol(int i)
{
	if (i < 0 || i >= m_atoms.size()) {
		return "";
	}

	return m_atoms[i].GetElementSymbol();
}
