#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "../Core/Ref.h"

class Connection : public RefCount
{
private:
    class Impl;
    Impl* m_imp;
    
protected:
    // Script Access Only
    Connection();
    ~Connection();
    
    bool Connect(const std::string& url);
    bool SendText(const std::string& text);
    bool SendJSON(const std::string& json);
    bool SendBinary(const char* binary, int size);
    bool SendImage(const std::string& filepath);
    bool Close();
};

#endif //_CONNECTION_H_
