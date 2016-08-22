/**
 * @file VolumeScatterPlot.cpp
 * ボリュームデータのプロットモジュール
 */

#include <string>
#include "VolumeScatterPlot.h"
#include "BufferVolumeData.h"
#include "BufferImageData.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>


namespace {

    void getMinMax(BufferVolumeData* volume, std::vector<float>& minVal, std::vector<float>& maxVal) {
        const int w = volume->Width();
        const int h = volume->Height();
        const int d = volume->Depth();
        const int c = volume->Component();
        const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());

        for (int p = 0; p < c; ++p) {
            minVal[p] =  FLT_MAX;
            maxVal[p] = -FLT_MAX;        
            for (int i = 0; i < w * h * d; ++i) {
                const float v = buffer[c * i + p];
                minVal[p] = fmin(v, minVal[p]);
                maxVal[p] = fmax(v, maxVal[p]);                
            }
        }
    }
    
    int getColor(int r, int g, int b, int a) {
        return ((r & 0xFF) << 24) | ((g & 0xFF) << 16) | ((b & 0xFF) << 8) | (a & 0xFF); 
    }
    
    void drawColor(unsigned char* buffer, int w, int h, int pos, int color) {
        if (pos < 0) { return; }
        if (pos > (w * h)) { return; }
        buffer[pos * 4 + 0] = static_cast<unsigned char>((color & 0xFF000000) >> 24);
        buffer[pos * 4 + 1] = static_cast<unsigned char>((color & 0x00FF0000) >> 16);
        buffer[pos * 4 + 2] = static_cast<unsigned char>((color & 0x0000FF00) >> 8);
        buffer[pos * 4 + 3] = static_cast<unsigned char>((color & 0x000000FF));
    }
    
    /**
     * draw cicle (filled) 
     * @param w image width
     * @param h image height
     * @param cx circle center x
     * @param cy circle center y
     * @parma r radius
     * @param color color
     */
    void drawCircle(unsigned char* buffer, int w, int h, int cx, int cy, int r, int color) {
        const int sx = cx - r;
        const int sy = cy - r;
        const int ex = cx + r;
        const int ey = cy + r;
        const float inv_root2 = 1.0f / 1.4142f;
        const float r2 = (r * inv_root2) * (r * inv_root2);
        for (int x = sx; x < ex; ++x) {
            for (int y = sy; y < ey; ++y) {
                const int len = (cx - x) * (cx - x) + (cy - y) * (cy - y);
                if (len < r2) {
                    const int pos = ((y * w) + x);
                    drawColor(buffer, w, h, pos, color);
                }
            }
        }
    }
    
    void swap(int& a, int& b) {
        int temp = a;
        b = a;
        a = temp;
    }
    
    /** 
     * draw line
     * @param w image width
     * @param h image height
     * @param x0 start x
     * @param y0 start y
     * @param x1 end x
     * @param y1 end y
     * @param color color
     */
    void drawLine(unsigned char* buffer, int w, int h, int x0, int y0, int x1, int y1, int color) {
        int sx = x0;
        int sy = y0;
        int ex = x1;
        int ey = y1;
        const int dx = abs(x1 - x0);
        if (dx == 0) {
            // vertial line
            for (int y = sy; y < ey; ++y) {
                drawColor(buffer, w, h, y * w + sx, color);
            }
            return;
        }
        const bool steep = abs(y1 - y0) > dx;
        if (steep) {
            swap(sx, sy);
            swap(ex, ey);
        }
        if (sx > ex) {
            swap(sx, sy);
            swap(ex, ey);
        }
        const int deltax = abs(ex - sx);
        const int deltay = abs(ey - sy);
        const int ystep = (sy < ey) ? 1 : -1;
        int error = deltax / 2;
        int y = sy;
        for (int x = sx; x < ex; ++x) {
            if (steep) {
                drawColor(buffer, w, h, x * w + y, color);
            } else {
                drawColor(buffer, w, h, y * w + x, color);
            }
            error -= deltay;
            if (error < 0) {
                y += ystep;
                error += deltax;
            }
        }
    }
    
    
    void plot(
            BufferImageData* dstImage,
            BufferVolumeData* volume, 
            std::vector<float>& minVal, 
            std::vector<float>& maxVal,
            int axis1,
            int axis2
            ) {
        const int w = volume->Width();
        const int h = volume->Height();
        const int d = volume->Depth();
        const int c = volume->Component();
        const double span1 = maxVal[axis1] - minVal[axis1];
        const double span2 = maxVal[axis2] - minVal[axis2];
        const float* buffer = static_cast<const float*>(volume->Buffer()->GetBuffer());
        
        unsigned char* imageBuffer = dstImage->ImageBuffer()->GetBuffer();
        const int imageWidth = dstImage->Width();
        const int imageHeight = dstImage->Height();

        for (int i = 0; i < w * h * d; ++i) {
            const int x = static_cast<int>((buffer[c * i + axis1] - minVal[axis1]) / span1 * 0xFF + 0.5f);
            const int y = static_cast<int>((buffer[c * i + axis2] - minVal[axis2]) / span2 * 0xFF + 0.5f);
            drawColor(imageBuffer, imageWidth, imageHeight, x + y * imageWidth, getColor(0, 0, 255, 255));
        }
    }
}

/// コンストラクタ
VolumeScatterPlot::VolumeScatterPlot()
{
    m_imagebuffer = BufferImageData::CreateInstance();
    m_imagebuffer->Create(BufferImageData::RGBA8, 256, 256);
}

/// デストラクタ
VolumeScatterPlot::~VolumeScatterPlot()
{
}

void VolumeScatterPlot::ClearImage()
{
    unsigned char* buffer = m_imagebuffer->ImageBuffer()->GetBuffer();
    for (int i = 0; i < m_imagebuffer->Bytes(); ++i) {
        buffer[i] = 0xFF;
    }
    const int w = m_imagebuffer->Width();
    const int h = m_imagebuffer->Height();
    /* test plot code
    for (int i = 0; i < 10000; ++i) {
        int px = static_cast<int>((float)rand() / RAND_MAX * 0xFF + 0.5f);
        int py = static_cast<int>((float)rand() / RAND_MAX * 0xFF + 0.5f);
        //drawCircle(buffer, w, h, px, py, 2, getColor(0, 0, 255, 255));
        drawColor(buffer, w, h, px + py * w, getColor(0, 0, 255, 255));
    }
    */
    int padding = 0;
    int max = 256;
    // vertial
    for (int x = padding; x < max; x+= 32) {
        drawLine(buffer, w, h, x, padding, x, max, getColor(200, 200, 200, 255));
    }
    // horizontal
    for (int y = padding; y < max; y += 32) {
        drawLine(buffer, w, h, padding, y, max, y, getColor(200, 200, 200, 255));
    }
}

/**
 * ボリュームモデルを画像にプロット
 * @param model 解析対象BufferVolumeData
 * @retval true 成功
 * @retval false 失敗
 */
bool VolumeScatterPlot::Execute(BufferVolumeData* volume, int axis1, int axis2)
{
    if(!volume || volume->GetType() != BufferData::TYPE_VOLUME) {
        fprintf(stderr,"Volume data not found.");
        return false;
    }

#define DUMPVALUE 1

    // dump volume info
    const int width = volume->Width();
    const int height = volume->Height();
    const int depth = volume->Depth();
    const int component = volume->Component();
    printf("Volume Size = (%dx%dx%dx%d)\n", width, height, depth, component); 

    std::vector<float> minVal;
    std::vector<float> maxVal;
    
    /*    
    m_hist.resize(compNum);
    for (int i = 0; i < compNum; ++i) {
        m_hist[i].resize(fNum);
    }
    */
    minVal.resize(component);
    maxVal.resize(component);
    
    // get min/max
    getMinMax(volume, minVal, maxVal);

#if DUMPVALUE
    // dump min/max
    for (int p = 0; p < component; ++p) {
        printf("%d - Min = %f / Max = %f\n", p, minVal[p], maxVal[p]);
    }
#endif

    ClearImage();
    plot(m_imagebuffer, volume, minVal, maxVal, axis1, axis2);
    
    
    
/*
    // histgram
    histgram(volume, minVal, maxVal, m_sigmaVal, m_hist);

#if DUMPVALUE
    // dump histgram
    for (int i = 0; i < fNum; ++i) {
        printf("%4d = %6.3f, %6.3f, %6.3f\n", i, m_hist[0][i], m_hist[1][i], m_hist[2][i]);
    }
#endif

    // clustering
    clustering(m_hist, minVal, maxVal, m_axisClusters);

#if DUMPVALUE
    // dump clustering
    for (int p = 0; p < 3; ++p) {
        int ax = m_axisClusters[p].size();
        for (int i = 0; i < ax; ++i) {
            printf("Cluster %3d-[%3d] = %6.3f, %6.3f, %6.3f\n", p, i, m_axisClusters[p][i].minValue, m_axisClusters[p][i].topValue, m_axisClusters[p][i].maxValue); 
        }
    }
#endif
   
    m_edgeCounts.resize(compNum - 1);
    for (int p = 0; p < compNum - 1; ++p) { // edge num = compNum - 1
        int pmax = m_axisClusters[p].size();
        m_edgeCounts[p].resize(pmax);
        for (int np = 0; np < pmax; ++np) {
            m_edgeCounts[p][np].resize(m_axisClusters[p+1].size());
        }
    
    }
    edgePower(volume, m_axisClusters, m_edgeCounts);

#if DUMPVALUE
    // dump edge value
    for (int p = 0; p < compNum - 1; ++p) {
        int en = m_edgeCounts[p].size();
        for (int i = 0; i < en; ++i) {
            int kn = m_edgeCounts[p][i].size();
            for (int k = 0; k < kn; ++k) {
                printf("Edge %3d[%3d]-%3d[%3d] = %d\n", p, i, p+1, k, m_edgeCounts[p][i][k]);
            }
        }
    }
#endif
 */
 
    return true;
}

BufferImageData* VolumeScatterPlot::GetImageBuffer()
{
    return m_imagebuffer;
}
