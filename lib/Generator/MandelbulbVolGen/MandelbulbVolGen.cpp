/**
 * @file MandelbulbVolGen.cpp
 * Mandelbulb Volume Generator
 */
#include <stdio.h>
#include <string.h>
#include "MandelbulbVolGen.h"

/// Constructor
MandelbulbVolGen::MandelbulbVolGen()
{
    m_volume = BufferVolumeData::CreateInstance();
    Clear();
}
/// Destructor
MandelbulbVolGen::~MandelbulbVolGen()
{
    Clear();
}

/// Clear members
void MandelbulbVolGen::Clear()
{
    m_volume->Clear();
}

/**
 * Generate Mandelbulb Volume
 * @param w width
 * @param h height
 * @param d depth
 * @param c component
 * @retval true 成功
 * @retval false 失敗
 */
bool MandelbulbVolGen::Generate(const int w, const int h, const int d, const int c)
{
    Clear();
    m_volume->Create(w, h, d, c);
    return true;
}

/**
 * VOLWidth取得
 * @retval int Width
 */
int MandelbulbVolGen::Width()
{
    return m_volume->Width();
}

/**
 * VOLHeight取得
 * @retval int Height
 */
int MandelbulbVolGen::Height()
{
    return m_volume->Height();
}

/**
 * VOLDepth取得
 * @retval int Depth
 */
int MandelbulbVolGen::Depth()
{
    return m_volume->Depth();
}

/**
 * VOLComponent取得
 * @retval int Component数
 */
int MandelbulbVolGen::Component()
{
    return m_volume->Component();
}

/**
 * VOLデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* MandelbulbVolGen::Buffer()
{
    return m_volume->Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *MandelbulbVolGen::VolumeData()
{
    return m_volume;
}

