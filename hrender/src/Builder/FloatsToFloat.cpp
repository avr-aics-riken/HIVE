#include "FloatsToFloat.h"

FloatsToFloat::FloatsToFloat(){
    m_volume = new BufferVolumeData();
}

int FloatsToFloat::Create(BufferVolumeData *volume, int offset){
    if (!volume)
        return 0;
    const int component = volume->Component();
    const int n = volume->Buffer()->GetNum() / component;
    // TODO: FIX
/*    m_volume->m_buffer = new FloatBuffer();
    m_volume->Buffer()->Create(n);
    printf("%8X :  %d %d %08X\n", m_volume, n, volume->Buffer(), m_volume->Buffer());
    for (int i = 0; i < n; ++i){
        (m_volume->Buffer()->GetBuffer())[i] = volume->Buffer()->GetBuffer()[component * i + offset];
    }*/
    return volume->Buffer()->GetNum();
}

int FloatsToFloat::Component()
{
    return 1;
}

BufferVolumeData* FloatsToFloat::VolumeData()
{
    return m_volume;
}

