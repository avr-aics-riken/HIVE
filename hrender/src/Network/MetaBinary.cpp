#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdint.h>

#include "../Buffer/Buffer.h"
#include "MetaBinary.h"

namespace {
    const std::string headerStr = "MetaBin:";
}

/**
 * default implementaion
 */
class MetaBinary::Impl
{
public:
    Impl()
    {
        m_byteBuffer = NULL;
    }
    
    ~Impl()
    {
    }
    
    bool Create(const std::string& json, const Buffer binary, int binarySize)
    {
        if (!binary) { return false; }
        
        m_byteBuffer = new ByteBuffer();
        if (m_byteBuffer) {
            m_byteBuffer->Create(headerStr.size() + 8 + json.size() + binarySize);
            int pos = 0;
            
            // header
            const int headerSize = static_cast<int>(headerStr.size());
            memcpy(&m_byteBuffer->GetBuffer()[pos], headerStr.c_str(), headerSize);
            pos += headerSize;
            
            // version (little endian)
            const unsigned int version = 1;
            memcpy(&m_byteBuffer->GetBuffer()[pos], &version, 4);
            pos += 4;
            
            // metadata size
            const int jsonSize = static_cast<int>(json.size());
            memcpy(&m_byteBuffer->GetBuffer()[pos], &jsonSize, 4);
            pos += 4;
            
            // metadata
            memcpy(&m_byteBuffer->GetBuffer()[pos], json.c_str(), jsonSize);
            pos += jsonSize;
            
            // binary
            memcpy(&m_byteBuffer->GetBuffer()[pos], binary, binarySize);
            return true;
        }
        return false;
    }
    
    const MetaBinary::Buffer BinaryBuffer() const
    {
        return reinterpret_cast<MetaBinary::Buffer>(m_byteBuffer->GetBuffer());
    }
    
    int BinaryBufferSize() const
    {
        return m_byteBuffer->GetNum();
    }
    
private:
    RefPtr<ByteBuffer> m_byteBuffer;
};

/// constructor.
MetaBinary::MetaBinary()
{
    m_imp = new MetaBinary::Impl();
}

/// destructor
MetaBinary::~MetaBinary()
{
    delete m_imp;
}

bool MetaBinary::Create(const std::string& json, const Buffer binary, int binarySize)
{
    return m_imp->Create(json, binary, binarySize);
}

const MetaBinary::Buffer MetaBinary::BinaryBuffer() const
{
    return m_imp->BinaryBuffer();
}

int MetaBinary::BinaryBufferSize() const
{
    return m_imp->BinaryBufferSize();
}
