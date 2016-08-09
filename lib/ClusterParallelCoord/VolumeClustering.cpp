/**
 * @file VolumeClustering.cpp
 * ボリュームデータのクラスタリングモジュール
 */

#include <string>
#include "VolumeClustering.h"

/// コンストラクタ
VolumeClustering::VolumeClustering()
{
    Clear();
}

/// デストラクタ
VolumeClustering::~VolumeClustering()
{
}

/// メンバクリア
void VolumeClustering::Clear() {

}

/**
 * ボリュームモデル解析
 * @param model 解析対象BufferVolumeData
 * @retval true 成功
 * @retval false 失敗
 */
bool VolumeClustering::Execute(BufferVolumeData* volume)
{
    if(!volume || volume->GetType() != BufferData::TYPE_VOLUME) {
        fprintf(stderr,"Volume data not found.");
        return false;
    }

    // TODO

    return true;
}

