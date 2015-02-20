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

    // Recive message and return its content.
    // Return fail(empty string) when timeout or recv() fails.
    std::string Recv();

    bool Close();
};

#endif //_CONNECTION_H_
