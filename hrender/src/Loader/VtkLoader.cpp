/**
 * @file SphLoader.cpp
 * VTKデータローダー
 */
#include <stdio.h>
#include <string.h>
#include "VtkLoader.h"
#include "tiny_vtkxml.h"
#include "BufferVolumeData.h"

/// コンストラクタ
VTKLoader::VTKLoader()
{
    Clear();
}

/// デストラクタ
VTKLoader::~VTKLoader()
{
    Clear();
}

/// メンバクリア
void VTKLoader::Clear()
{
    m_volume = 0;
}

/**
 * VTKデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool VTKLoader::Load(const char* filename)
{
    Clear();
    m_volume = new BufferVolumeData();

	// @todo
	return false;
}

/**
 * VTKWidth取得
 * @retval int Width
 */
int VTKLoader::Width()    {
    return m_volume->Width();
}

/**
 * VTKHeight取得
 * @retval int Height
 */
int VTKLoader::Height()   {
    return m_volume->Height();
}

/**
 * VTKDepth取得
 * @retval int Depth
 */
int VTKLoader::Depth()    {
    return m_volume->Depth();
}

/**
 * VTKComponent取得
 * @retval int Component数
 */
int VTKLoader::Component() {
    return m_volume->Component();
}

/**
 * VTKデータバッファ参照取得
 * @retval FloatBuffer* FloatBufferアドレス
 */
FloatBuffer* VTKLoader::Buffer() {
    return m_volume->Buffer();
}

/**
 * VolumeData参照取得
 * @retval BufferVolumeData* VolumeData参照
 */
BufferVolumeData *VTKLoader::VolumeData()
{
    return m_volume;
}
