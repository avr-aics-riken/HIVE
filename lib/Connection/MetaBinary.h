/**
 * @file MetaBinary.h
 * メタバイナリ
 */
#ifndef _METABINARY_H_
#define _METABINARY_H_

#include "../Core/Ref.h"

/**
 * メタバイナリ
 */
class MetaBinary : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    typedef void* Buffer;
    
    MetaBinary();
    ~MetaBinary();
    
    bool Create(const std::string& json, const Buffer binary, int binarySize);
    
    const Buffer BinaryBuffer() const;

    int BinaryBufferSize() const;
};

#endif //_METABINARY_H_
