#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "../Core/Ref.h"

class ConnectionIF
{
public:
    typedef void* Binary;
    virtual bool Connect(const std::string& url) = 0;
    virtual bool SendText(const std::string& text) = 0;
    virtual bool SendJSON(const std::string& json) = 0;
    virtual bool SendBinary(const Binary binary, int size) = 0;
    virtual bool SendImage(const std::string& filepath) = 0;
    // Recive message and return its content to `msg`.
    // Return fail when timeout or recv() fails.
    virtual std::string Recv() = 0;
    virtual bool Close() = 0;
    
    virtual ~ConnectionIF() {}
protected:
    ConnectionIF() {}
};

class Connection : public RefCount, public ConnectionIF
{
private:
    ConnectionIF* m_imp;
    
protected:
    // Script Access Only
    Connection();
    ~Connection();
    
    bool Connect(const std::string& url);
    bool SendText(const std::string& text);
    bool SendJSON(const std::string& json);
    bool SendBinary(const Binary binary, int size);
    bool SendImage(const std::string& filepath);
    std::string Recv();
    bool Close();
};

#endif //_CONNECTION_H_
