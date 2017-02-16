/**
 * @file ImageFilter.h
 * イメージフィルター
 */
#ifndef _IMAGEFILTER_H_
#define _IMAGEFILTER_H_

#include "Ref.h"

class BufferImageData;

/**
 * イメージフィルター
 */
class ImageFilter : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    enum Operation {
        ADD = 1,
        SUBSTRACT,
        MULTIPLY,
        DIVIDE
    };

    ImageFilter();
    ~ImageFilter();

    BufferImageData* Filter(unsigned int operation, BufferImageData* left, BufferImageData* right, float opacity);
};

#endif //_IMAGEFILTER_H_
