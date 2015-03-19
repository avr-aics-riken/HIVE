/**
 * @file FloatsToFloat.cpp   
 * 複数Floatデータ再構築モジュール
 */

#include "FloatsToFloat.h"

/// コンストラクタ
FloatsToFloat::FloatsToFloat(){
    m_volume = new BufferVolumeData();
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
    const int component = volume->Component();
    const int n = volume->Buffer()->GetNum() / component;
    // TODO: FIX
/*    m_volume->m_buffer = new FloatBuffer();
    m_volume->Buffer()->Create(n);
    printf("%p :  %d %p %p\n", m_volume, n, volume->Buffer(), m_volume->Buffer());
    for (int i = 0; i < n; ++i){
        (m_volume->Buffer()->GetBuffer())[i] = volume->Buffer()->GetBuffer()[component * i + offset];
    }*/
    return volume->Buffer()->GetNum();
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
    return m_volume;
}

