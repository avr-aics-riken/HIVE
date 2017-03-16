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
        float factor)
    {
        if (operation == ImageFilter::ADD) {
            // const float leftAlpha = (stride == 3) ? 1.0 : left[3];
            // const float rightAlpha = (stride == 3) ? 1.0 : right[3]; 
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                dst[ir] = (std::min)(static_cast<int>(left[ir] + right[ir] * factor), 0xFF);
                dst[ig] = (std::min)(static_cast<int>(left[ig] + right[ig] * factor), 0xFF);
                dst[ib] = (std::min)(static_cast<int>(left[ib] + right[ib] * factor), 0xFF);
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
                dst[ir] = (std::min)(static_cast<int>(rr * factor + left[ir] * (1.0f - factor)), 0xFF);
                dst[ig] = (std::min)(static_cast<int>(gg * factor + left[ig] * (1.0f - factor)), 0xFF);
                dst[ib] = (std::min)(static_cast<int>(bb * factor + left[ib] * (1.0f - factor)), 0xFF);
            }
        } else if (operation == ImageFilter::MULTIPLY) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                dst[ir] = (std::min)(static_cast<int>(left[ir] * (0xFF + (right[ir] - 0xFF) * factor) / 0xFF), 0xFF);
                dst[ig] = (std::min)(static_cast<int>(left[ig] * (0xFF + (right[ig] - 0xFF) * factor) / 0xFF), 0xFF);
                dst[ib] = (std::min)(static_cast<int>(left[ib] * (0xFF + (right[ib] - 0xFF) * factor) / 0xFF), 0xFF);
            }
        } else if (operation == ImageFilter::AVERAGE) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                dst[ir] = (std::min)(static_cast<int>( (left[ir] + right[ir]) / 2.0 * factor + left[ir] * (1.0 - factor)), 0xFF);
                dst[ig] = (std::min)(static_cast<int>( (left[ig] + right[ig]) / 2.0 * factor + left[ig] * (1.0 - factor)), 0xFF);
                dst[ib] = (std::min)(static_cast<int>( (left[ib] + right[ib]) / 2.0 * factor + left[ib] * (1.0 - factor)), 0xFF);
            }
        }
    }
    
    void FilterFloatBuffer(
        unsigned int operation,
        float *dst,
        const float* left,
        const float* right,
        const int stride,
        const int length,
        float factor)
    {
        if (operation == ImageFilter::ADD) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                const int src_ir = (stride == 1) ? ir : ir;
                const int src_ig = (stride == 1) ? ir : ig;
                const int src_ib = (stride == 1) ? ir : ib;
                dst[ir] = (std::min)(left[src_ir] + right[src_ir] * factor, 1.0f);
                dst[ig] = (std::min)(left[src_ig] + right[src_ig] * factor, 1.0f);
                dst[ib] = (std::min)(left[src_ib] + right[src_ib] * factor, 1.0f);
            }
        } else if (operation == ImageFilter::SUBSTRACT) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                const int src_ir = (stride == 1) ? ir : ir;
                const int src_ig = (stride == 1) ? ir : ig;
                const int src_ib = (stride == 1) ? ir : ib;
                const float rr = (std::max)(left[src_ir] + right[src_ir] - 1.0f, 0.0f);
                const float gg = (std::max)(left[src_ig] + right[src_ig] - 1.0f, 0.0f);
                const float bb = (std::max)(left[src_ib] + right[src_ib] - 1.0f, 0.0f);
                dst[ir] = (std::min)(rr * factor + left[src_ir] * (1.0f - factor), 1.0f);
                dst[ig] = (std::min)(gg * factor + left[src_ig] * (1.0f - factor), 1.0f);
                dst[ib] = (std::min)(bb * factor + left[src_ib] * (1.0f - factor), 1.0f);
            }
        } else if (operation == ImageFilter::MULTIPLY) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                const int src_ir = (stride == 1) ? ir : ir;
                const int src_ig = (stride == 1) ? ir : ig;
                const int src_ib = (stride == 1) ? ir : ib;
                dst[ir] = (std::min)(left[src_ir] * (1.0f + (right[src_ir] - 1.0f) * factor), 1.0f);
                dst[ig] = (std::min)(left[src_ig] * (1.0f + (right[src_ig] - 1.0f) * factor), 1.0f);
                dst[ib] = (std::min)(left[src_ib] * (1.0f + (right[src_ib] - 1.0f) * factor), 1.0f);
            }
        } else if (operation == ImageFilter::AVERAGE) {
            for (int i = 0; i < length; ++i)
            {
                const int ir = i * stride + 0;
                const int ig = i * stride + 1;
                const int ib = i * stride + 2;
                const int src_ir = (stride == 1) ? ir : ir;
                const int src_ig = (stride == 1) ? ir : ig;
                const int src_ib = (stride == 1) ? ir : ib;
                dst[ir] = (std::min)((left[src_ir] + right[src_ir]) / 2.0f * factor + left[src_ir] * (1.0f - factor), 1.0f);
                dst[ig] = (std::min)((left[src_ig] + right[src_ig]) / 2.0f * factor + left[src_ig] * (1.0f - factor), 1.0f);
                dst[ib] = (std::min)((left[src_ib] + right[src_ib]) / 2.0f * factor + left[src_ib] * (1.0f - factor), 1.0f);
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
    
    BufferImageData* Filter(unsigned int operation, BufferImageData* left, BufferImageData* right, float factor)
    {
        if (!left || !right) return NULL;        
        if (left->Width() != right->Width() || left->Height() != right->Height()) return NULL;
        if (left->Format() != right->Format()) return NULL;
        
        BufferImageData* dst = BufferImageData::CreateInstance();
        dst->Clear();
        
        const int width = left->Width();
        const int height = left->Height();
        dst->Create(left->Format(), width, height);
        
        if (dst->ImageBuffer()) {
            for (int i = 0; i < dst->ImageBuffer()->GetNum(); ++i)
            {
                dst->ImageBuffer()->GetBuffer()[i] = 0xFF;
            }
        }
        if (dst->FloatImageBuffer())
        {
            for (int i = 0; i < dst->FloatImageBuffer()->GetNum(); ++i)
            {
                dst->FloatImageBuffer()->GetBuffer()[i] = 1.0f;
            }
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
                factor);
        } else if (left->Format() == BufferImageData::RGBA8) {
            FilterByteBuffer(
                operation,
                dst->ImageBuffer()->GetBuffer(), 
                left->ImageBuffer()->GetBuffer(),
                right->ImageBuffer()->GetBuffer(),
                4,
                dst->ImageBuffer()->GetNum() / 4,
                factor);
        } else if (left->Format() == BufferImageData::RGBA32F) {
            FilterFloatBuffer(
                operation,
                dst->FloatImageBuffer()->GetBuffer(),
                left->FloatImageBuffer()->GetBuffer(),
                right->FloatImageBuffer()->GetBuffer(),
                4,
                dst->FloatImageBuffer()->GetNum() / 4,
                factor);
        } else if (left->Format() == BufferImageData::R32F) {
            FilterFloatBuffer(
                operation,
                dst->FloatImageBuffer()->GetBuffer(),
                left->FloatImageBuffer()->GetBuffer(),
                right->FloatImageBuffer()->GetBuffer(),
                1,
                dst->FloatImageBuffer()->GetNum(),
                factor);
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

BufferImageData* ImageFilter::Filter(unsigned int operation, BufferImageData* left, BufferImageData* right, float factor)
{
    return m_imp->Filter(operation, left, right, factor);
}
