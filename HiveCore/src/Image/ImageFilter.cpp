/**
 * @file ImageFilter.cpp
 * イメージフィルター
 */
#include "ImageFilter.h"
#include <stdio.h>
#include <string>
#include <algorithm>
#include "BufferImageData.h"
#include "Buffer.h"


namespace
{
    
    void FilterByteBuffer(
        unsigned int operation,
        unsigned char *dst,
        const unsigned char* left,
        const unsigned char* right,
        const int stride,
        const int length,
        float opacity)
    {
        if (operation == ImageFilter::ADD) {
            // const float leftAlpha = (stride == 3) ? 1.0 : left[3];
            // const float rightAlpha = (stride == 3) ? 1.0 : right[3]; 
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                dst[ir] = (std::min)(static_cast<int>(left[ir] + right[ir] * opacity), 0xFF);
                dst[ig] = (std::min)(static_cast<int>(left[ig] + right[ig] * opacity), 0xFF);
                dst[ib] = (std::min)(static_cast<int>(left[ib] + right[ib] * opacity), 0xFF);
            }
        } else if (operation == ImageFilter::SUBSTRACT) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                const int rr = (std::max)(left[ir] + right[ir] - 0xFF, 0);
                const int gg = (std::max)(left[ig] + right[ig] - 0xFF, 0);
                const int bb = (std::max)(left[ib] + right[ib] - 0xFF, 0);
                dst[ir] = (std::min)(static_cast<int>(rr * opacity + left[ir] * (1.0f - opacity)), 0xFF);
                dst[ig] = (std::min)(static_cast<int>(gg * opacity + left[ig] * (1.0f - opacity)), 0xFF);
                dst[ib] = (std::min)(static_cast<int>(bb * opacity + left[ib] * (1.0f - opacity)), 0xFF);
            }
        } else if (operation == ImageFilter::MULTIPLY) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                dst[ir] = (std::min)(static_cast<int>(left[ir] * (0xFF + (right[ir] - 0xFF) * opacity) / 0xFF), 0xFF);
                dst[ig] = (std::min)(static_cast<int>(left[ig] * (0xFF + (right[ig] - 0xFF) * opacity) / 0xFF), 0xFF);
                dst[ib] = (std::min)(static_cast<int>(left[ib] * (0xFF + (right[ib] - 0xFF) * opacity) / 0xFF), 0xFF);
            }
        } else if (operation == ImageFilter::AVERAGE) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                dst[ir] = (std::min)(static_cast<int>( (left[ir] + right[ir]) / 2.0 * opacity + left[ir] * (1.0 - opacity)), 0xFF);
                dst[ig] = (std::min)(static_cast<int>( (left[ig] + right[ig]) / 2.0 * opacity + left[ig] * (1.0 - opacity)), 0xFF);
                dst[ib] = (std::min)(static_cast<int>( (left[ib] + right[ib]) / 2.0 * opacity + left[ib] * (1.0 - opacity)), 0xFF);
            }
        }
    }
}

class ImageFilter::Impl
{
private:
    RefPtr<BufferImageData> m_image;
    
public:
    /// コンストラクタ
    Impl() {
        //m_image = BufferImageData::CreateInstance();
    }

    /// デストラクタ
    ~Impl() {}
    
    BufferImageData* Filter(unsigned int operation, BufferImageData* left, BufferImageData* right, float opacity)
    {
        if (!left || !right) return NULL;        
        if (left->Width() != right->Width() || left->Height() != right->Height()) return NULL;
        if (left->Format() != right->Format()) return NULL;
        
        BufferImageData* dst = BufferImageData::CreateInstance();
        dst->Clear();
        
        const int width = left->Width();
        const int height = left->Height();
        dst->Create(left->Format(), width, height);
        
        for (int i = 0; i < dst->ImageBuffer()->GetNum(); ++i)
        {
            dst->ImageBuffer()->GetBuffer()[i] = 0xFF;
        }
        
        /*
        RGB8,
        RGBA8,
        */
        if (dst->Format() == BufferImageData::RGB8) {
            FilterByteBuffer(
                operation,
                dst->ImageBuffer()->GetBuffer(), 
                left->ImageBuffer()->GetBuffer(),
                right->ImageBuffer()->GetBuffer(),
                3,
                dst->ImageBuffer()->GetNum() / 3,
                opacity);
        } else if (left->Format() == BufferImageData::RGBA8) {
            FilterByteBuffer(
                operation,
                dst->ImageBuffer()->GetBuffer(), 
                left->ImageBuffer()->GetBuffer(),
                right->ImageBuffer()->GetBuffer(),
                4,
                dst->ImageBuffer()->GetNum() / 4,
                opacity);
        }
        
        return dst;
    }
};

ImageFilter::ImageFilter()
{
    m_imp = new ImageFilter::Impl();
}

ImageFilter::~ImageFilter()
{
    delete m_imp;
}

BufferImageData* ImageFilter::Filter(unsigned int operation, BufferImageData* left, BufferImageData* right, float opacity)
{
    return m_imp->Filter(operation, left, right, opacity);
}
