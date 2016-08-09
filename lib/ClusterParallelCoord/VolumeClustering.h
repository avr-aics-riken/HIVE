/**
 * @file VolumeClustering.h
 * ボリュームデータのクラスタリングモジュール
 */

#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

class VolumeClustering : public RefCount
{
public:
    VolumeClustering();
    ~VolumeClustering();

    void Clear();

    bool Execute(BufferVolumeData* volume);
};

