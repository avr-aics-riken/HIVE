/**
 * @file BcmLoader.cpp
 * BCMローダー
 */
#ifndef HIVE_WITH_BCMTOOLS
#error "HIVE_WITH_BCMTOOLS must be defined when you compile BCMLoader module"
#endif

#include <mpi.h>

#include <stdio.h>
#include <string.h>

#include <string>

#include "BcmLoader.h"
//#include "SimpleVOL.h"

#include "BCMTools.h"
#include "BCMOctree.h"
//#include "PolygonBBoxDivider.h"

//#include "Config.h"

/// コンストラクタ
BCMLoader::BCMLoader()
{
    Clear();
}

/// デストラクタ
BCMLoader::~BCMLoader()
{
    Clear();
}

/// クリア
void BCMLoader::Clear()
{
    m_volume.Clear();
}

/**
 * BCMデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool BCMLoader::Load(const char* filename)
{
    Clear();

	// Assume MPI_Init() has been called in hrender::main()

	int rank = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	BCMOctree* tree;
	if (rank == 0) {
		int N = 1; // @fixme
		RootGrid* rootGrid = new RootGrid(N);
		Divider* divider = NULL; // @fixme

		//Divider* divider = new PolygonBBoxDivider(conf.origin, conf.rootLength,
		//										  rootGrid, conf.minLevel, pl,
		//										  conf.polygonGroupList,
		//										  conf.boundingBoxList,
		//										  (double)conf.vc/conf.size);

		BCMOctree::Ordering ordering = BCMOctree::Z; // @fixme

		tree = new BCMOctree(rootGrid, divider, ordering);
	} else {
		tree = BCMOctree::ReceiveFromMaster();
	}
	// @todo {}

    return false;
}

/**
 * BCMWidth取得
 * @retval int Width
 */
int BCMLoader::Width()    {
    return m_volume.Width();
}

/**
 * BCMHeight取得
 * @retval int Height
 */
int BCMLoader::Height()   {
    return m_volume.Height();
}

/**
 * BCMDepth取得
 * @retval int Depth
 */
int BCMLoader::Depth()    {
    return m_volume.Depth();
}

/**
 * BCMComponent取得
 * @retval int Component数
 */
int BCMLoader::Component() {
    return m_volume.Component();
}

/**
 * BCMデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* BCMLoader::Buffer() {
    return m_volume.Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *BCMLoader::VolumeData()
{
    return &m_volume;
}



