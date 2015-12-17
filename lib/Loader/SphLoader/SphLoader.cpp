/**
 * @file SphLoader.cpp
 * SPHデータローダー
 */
#include <stdio.h>
#include <string.h>
#include "SphLoader.h"
#include "SimpleSPH.h"
#include "BufferVolumeData.h"

/// コンストラクタ
SPHLoader::SPHLoader()
{
    Clear();
}

/// デストラクタ
SPHLoader::~SPHLoader()
{
    Clear();
}

/// メンバクリア
void SPHLoader::Clear()
{
    m_volume = 0;
}

/**
 * SPHデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool SPHLoader::Load(const char* filename)
{
    Clear();
    m_volume = BufferVolumeData::CreateInstance();

    SimpleSPH sph;
    const float* buf = sph.Load(filename);
    if (!buf) {
        fprintf(stderr,"Failed to load SPH volume: %s\n", filename);
        return false;
    }
    const int w = sph.GetDim(0);
    const int h = sph.GetDim(1);
    const int d = sph.GetDim(2);
    const int c = sph.GetComponent();
    m_volume->Create(w, h, d, c);
    FloatBuffer* buffer = m_volume->Buffer();
    const int fnum = w * h * d * c;
    memcpy(buffer->GetBuffer(), buf, fnum * sizeof(float));
    delete [] buf;

    m_origin[0] = sph.GetOrigin(0);
    m_origin[1] = sph.GetOrigin(1);
    m_origin[2] = sph.GetOrigin(2);
    m_pitch[0] = sph.GetPitch(0);
    m_pitch[1] = sph.GetPitch(1);
    m_pitch[2] = sph.GetPitch(2);
    m_time = sph.GetTime();
    m_step = sph.GetStep();

    return true;
}

/**
 * SPHWidth取得
 * @retval int Width
 */
int SPHLoader::Width()    {
    return m_volume->Width();
}

/**
 * SPHHeight取得
 * @retval int Height
 */
int SPHLoader::Height()   {
    return m_volume->Height();
}

/**
 * SPHDepth取得
 * @retval int Depth
 */
int SPHLoader::Depth()    {
    return m_volume->Depth();
}

/**
 * SPHComponent取得
 * @retval int Component数
 */
int SPHLoader::Component() {
    return m_volume->Component();
}

/**
 * SPHデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* SPHLoader::Buffer() {
    return m_volume->Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *SPHLoader::VolumeData()
{
    return m_volume;
}
