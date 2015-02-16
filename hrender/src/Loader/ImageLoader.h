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
    ImageLoader();
    ~ImageLoader();
    bool Load(const char* filename);
    BufferImageData *ImageData();
};

#endif //_IMAGELOADER_H_
