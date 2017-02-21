/**
 * @file ImageLoader.h
 * 画像ローダ
 */
#ifndef _IMAGELOADER_H_
#define _IMAGELOADER_H_

#include "Ref.h"

class BufferImageData;

/**
 * 画像ローダ
 */
class ImageLoader : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    typedef void* Buffer;
    
    ImageLoader();
    ~ImageLoader();
    bool Load(const char* filename);
    bool LoadMemory(unsigned int format, const Buffer buffer,unsigned int bufferSize);
    BufferImageData *ImageData();
    
    /// @retval no compressed buffer
    Buffer ImageBuffer() const;
    
    /// @retval no compressed buffer size
    int ImageBufferSize() const;
    
    bool LoadRawFromPointer(int width, int height, int color, int bit, void* ptr);
    
    /// deep copy
    bool LoadImageData(BufferImageData* imageData);

};

#endif //_IMAGELOADER_H_
