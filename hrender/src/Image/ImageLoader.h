#ifndef _IMAGELOADER_H_
#define _IMAGELOADER_H_

#include "Ref.h"

class BufferImageData;

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
    BufferImageData *ImageData();
    
    /// @retval no compressed buffer
    const Buffer ImageBuffer() const;
    
    /// @retval no compressed buffer size
    int ImageBufferSize() const;
};

#endif //_IMAGELOADER_H_
