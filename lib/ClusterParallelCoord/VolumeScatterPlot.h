/**
 * @file VolumeScatterPlot.h
 * ボリュームデータのプロットモジュール
 */
#ifndef _VOLUMESCATTERPLOT_H_
#define _VOLUMESCATTERPLOT_H_


#include "Ref.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

#include <vector>

class BufferImageData;

class VolumeScatterPlot : public RefCount
{
public:
    VolumeScatterPlot();
    ~VolumeScatterPlot();

    void ClearImage();
    bool Execute(BufferVolumeData* volume, int axis1, int axis2);

    BufferImageData* GetImageBuffer();

private:
    RefPtr<BufferImageData> m_imagebuffer;
};

#endif //_VOLUMESCATTERPLOT_H_
