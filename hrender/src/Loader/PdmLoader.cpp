/**
 * @file PdmLoader.cpp
 * PDMデータローダー
 */
#include "PdmLoader.h"

#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile CDMLoader module"
#endif

#include <cstring>

#include "PDMlib.h"

/// コンストラクタ
PDMLoader::PDMLoader(){}
/// デストラクタ
PDMLoader::~PDMLoader(){};

/// メンバクリア
void PDMLoader::Clear()
{
	points.Clear();
}

/**
 * PDMデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool PDMLoader::Load(const char* filename){
	Clear();

	// @todo {}
	return false;
}

/**
 * 点データ取得
 * @retval BufferPointData* 点データバッファへの参照
 */
BufferPointData *PDMLoader::PointData()
{
	return &points;
}
