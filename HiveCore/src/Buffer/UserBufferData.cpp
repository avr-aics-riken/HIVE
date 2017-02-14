#include "UserBufferData.h"
#include "BufferData.h"
#include <map>
#include <string>
#include <algorithm>

class UserBufferData::Impl
{
private: 
    std::map<std::string, RefPtr<BufferData> > m_buffer;

public:
    Impl()
    {
    }

    Impl(const UserBufferData* inst)
    {
        m_buffer  = inst->m_imp->m_buffer;
    }
  
    bool SetBufferData(const char* id, BufferData* data)
    {
        std::string dataID(id);
        m_buffer[dataID] = data;
        return true;
    }
    
    BufferData* GetBufferData(const char* id)
    {
        std::string dataID(id);
        if (m_buffer.find(dataID) != m_buffer.end()) {
            BufferData* data = m_buffer[dataID].Get();
            return data;
        }
        return NULL;
    }
    
    void Clear()
    {
        m_buffer.clear();
    }
};

UserBufferData::UserBufferData() : m_imp(new Impl()) {}

UserBufferData::UserBufferData(const UserBufferData* inst)
{
    m_imp = new UserBufferData::Impl(inst);
}

UserBufferData::~UserBufferData()
{
    delete m_imp;
}

void UserBufferData::Clear()
{
    m_imp->Clear();
}

bool UserBufferData::SetBufferData(const char* id, BufferData* data)
{
    return m_imp->SetBufferData(id, data);
}

BufferData* UserBufferData::GetBufferData(const char* id)
{
    return m_imp->GetBufferData(id);
}
