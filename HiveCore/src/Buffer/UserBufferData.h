/**
 * @file UserBufferData.h
 * ユーザーバッファデータ
 * 一時的に任意のBufferDataを格納する
 */
#ifndef _USERBUFFERDATA_H_
#define _USERBUFFERDATA_H_

#include "Ref.h"

class BufferData;

/**
 * ユーザーバッファデータ
 */
class UserBufferData : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
public:
    UserBufferData(const UserBufferData* inst);
    UserBufferData();
    ~UserBufferData();
    
    bool SetBufferData(const char* id, BufferData* data);
    BufferData* GetBufferData(const char* id);
    void Clear();
};

#endif //_USERBUFFERDATA_H_
