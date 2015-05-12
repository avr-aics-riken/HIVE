/**
 * @file FloatsToFloat.h
 * 複数Floatデータ再構築モジュール
 */

#ifndef _FLOATSTOFLOAT_H_
#define _FLOATSTOFLOAT_H_

#include "Ref.h"
#include "Buffer.h"
#include <vector>
#include "BufferVolumeData.h"

/**
 * 複数Floatデータ再構築モジュール
 */
class FloatsToFloat : public RefCount
{
private:
    BufferVolumeData* m_volume;
public:
    FloatsToFloat();
    int Create(BufferVolumeData *volume, int offset);
    int Component();
    BufferVolumeData* VolumeData();
};

#endif //_FLOATSTOFLOAT_H_

