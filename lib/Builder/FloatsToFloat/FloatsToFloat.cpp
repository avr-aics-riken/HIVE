/**
 * @file FloatsToFloat.cpp   
 * 複数Floatデータ再構築モジュール
 */

#include "FloatsToFloat.h"

/// コンストラクタ
FloatsToFloat::FloatsToFloat(){
    m_volume = 0;
}

/**
 * BufferVolumeData再構築
 * @param volume 再構築対象のBufferVolumeData
 * @param offset 再構築対象のバッファオフセット
 * @return 作成されたバッファ数
 */
int FloatsToFloat::Create(BufferVolumeData *volume, int offset){
    if (!volume)
        return 0;
    m_offset = offset;
    m_volume = volume;
}

/// コンポーネント数取得(1固定)
int FloatsToFloat::Component()
{
    return 1;
}

/**
 * 再構築されたバッファへの参照
 * @retval m_volume VolumeDataへの参照
 */

BufferVolumeData* FloatsToFloat::VolumeData()
{
    if (!m_volume) {
        return 0;
    }
    
    const int component = m_volume->Component();
    const int n = m_volume->Buffer()->GetNum() / component;

    int ofs = m_offset;
    if (component <= m_offset) {
        ofs = component - 1;
    }
   
    BufferVolumeData* volume = new BufferVolumeData();
    const int w = m_volume->Width();
    const int h = m_volume->Height();
    const int d = m_volume->Depth();
    volume->Create(w, h, d, 1);
    printf("FloatsToFloat : %d %d %d\n", w, h, d);
    
    float* tarbuf = volume->Buffer()->GetBuffer();
    const float* srcbuf = m_volume->Buffer()->GetBuffer();
    
    for (int i = 0; i < n; ++i){
        tarbuf[i] = srcbuf[component * i + ofs];
    }
    return volume;
}

