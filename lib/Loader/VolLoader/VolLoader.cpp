/**
 * @file VolLoader.cpp
 * VOLデータローダー
 */
#include <stdio.h>
#include <string.h>
#include "VolLoader.h"
#include "SimpleVOL.h"

/// コンストラクタ
VOLLoader::VOLLoader()
{
    Clear();
}
/// デストラクタ
VOLLoader::~VOLLoader()
{
    Clear();
}

/// メンバクリア
void VOLLoader::Clear()
{
    m_volume->Clear();
}

/**
 * VOLデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool VOLLoader::Load(const char* filename)
{
    Clear();

    SimpleVOL vol;
    const float* buf = vol.Load(filename);
    if (!buf) {
        fprintf(stderr,"Failed to load VOL volume: %s\n", filename);
        return false;
    }
    
    const int w = vol.GetDim(0);
    const int h = vol.GetDim(1);
    const int d = vol.GetDim(2);
    const int c = vol.GetComponent();
    m_volume->Create(w, h, d, c);
    memcpy(m_volume->Buffer()->GetBuffer(), buf, w * h * d * c * sizeof(float));
    delete [] buf;

    return true;
}

/**
 * VOLWidth取得
 * @retval int Width
 */
int VOLLoader::Width()    {
    return m_volume->Width();
}

/**
 * VOLHeight取得
 * @retval int Height
 */
int VOLLoader::Height()   {
    return m_volume->Height();
}

/**
 * VOLDepth取得
 * @retval int Depth
 */
int VOLLoader::Depth()    {
    return m_volume->Depth();
}

/**
 * VOLComponent取得
 * @retval int Component数
 */
int VOLLoader::Component() {
    return m_volume->Component();
}

/**
 * VOLデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* VOLLoader::Buffer() {
    return m_volume->Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *VOLLoader::VolumeData()
{
    return m_volume;
}

