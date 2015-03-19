/**
 * @file HdmLoader.cpp
 * HDMデータローダー
 */


#ifndef HIVE_WITH_HDMLIB
#error "HIVE_WITH_HDMLIB must be defined when you compile HDMLoader module"
#endif

#include <mpi.h> // must include mpi.h befor stdio.h for Intel MPI compiler.

#include <stdio.h>
#include <string.h>

#include <string>

#include "HdmLoader.h"
#include "SimpleVOL.h"


#include "BlockFactory.h"
#include "Block.h"
#include "BlockManager.h"
#include "Scalar3D.h"

//#include "LeafBlockLoader.h"
#include "BCMFileLoader.h"
#include "BCMFileSaver.h"

// @todo { implement }

/// コンストラクタ
HDMLoader::HDMLoader()
{
    Clear();
}

/// デストラクタ
HDMLoader::~HDMLoader()
{
    Clear();
}

/// ボリュームクリア
void HDMLoader::Clear()
{
    m_volume.Clear();
}

/**
 * HDMデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool HDMLoader::Load(const char* cellidFilename, const char* dataFilename)
{
    Clear();

	// cellid.bcm
	BCMFileIO::BCMFileLoader loader(cellidFilename, /* bscatrer */NULL);

	BlockManager& blockManager = BlockManager::getInstance();
    blockManager.printBlockLayoutInfo();

	// data.bcm
	if( !loader.LoadAdditionalIndex(dataFilename) ){
		printf("err : Load File Error %s)\n", dataFilename);
	    return false;
	}



    return false;
}

/**
 * HDMWidth取得
 * @retval int HDMWidth
 */
int HDMLoader::Width()    {
    return m_volume.Width();
}

/**
 * HDMHeight取得
 * @retval int HDMHeight
 */
int HDMLoader::Height()   {
    return m_volume.Height();
}

/**
 * HDMDepth取得
 * @retval int HDMDepth
 */
int HDMLoader::Depth()    {
    return m_volume.Depth();
}

/**
 * HDMComponent取得
 * @retval int Component数
 */
int HDMLoader::Component() {
    return m_volume.Component();
}

/**
 * HDMデータバッファ取得
 * @retval FloatBuffer* HDMデータバッファアドレス
 */
FloatBuffer* HDMLoader::Buffer() {
    return m_volume.Buffer();
}

/**
 * VolumeData取得
 * @retval VolumeData* VolumeDataアドレス
 */
BufferVolumeData *HDMLoader::VolumeData()
{
    return &m_volume;
}

