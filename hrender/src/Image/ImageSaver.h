#ifndef _IMAGESAVER_H_
#define _IMAGESAVER_H_

#include "Ref.h"

class BufferImageData;

class ImageSaver : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    ImageSaver();
    ~ImageSaver();
    bool Save(const char* filename, BufferImageData* data);
};

#endif //_IMAGESAVER_H_
