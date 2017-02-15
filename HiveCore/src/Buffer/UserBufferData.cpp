#include "UserBufferData.h"
#include "BufferData.h"
#include <map>
#include <string>
#include <algorithm>

class UserBufferData::Impl
{
private: 
    typedef std::map<std::string, RefPtr<BufferData> > BufferDataMap; 
    BufferDataMap m_buffer;

public:
    Impl()
    {
    }

    Impl(const UserBufferData* inst)
    {
        m_buffer  = inst->m_imp->m_buffer;
    }
  
    bool SetBufferData(const char* name, BufferData* data)
    {
        std::string dataName(name);
        m_buffer[dataName] = data;
        return true;
    }
    
    BufferData* GetBufferData(const char* name)
    {
        std::string dataName(name);
        if (m_buffer.find(dataName) != m_buffer.end()) {
            BufferData* data = m_buffer[dataName].Get();
            return data;
        }
        return NULL;
    }
    
    bool DeleteBufferData(const char* name)
    {
        std::string dataName(name);
        BufferDataMap::iterator it = m_buffer.find(dataName);
        if (it != m_buffer.end()) {
            m_buffer.erase(it);
            return true;
        }
        return false;
    }
    
    void GetBufferDataNames(std::vector<std::string>& dst) const
    {
        BufferDataMap::const_iterator it = m_buffer.begin();
        for (; it != m_buffer.end(); ++it) {
            dst.push_back(it->first);
        }
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

bool UserBufferData::DeleteBufferData(const char* id)
{
    return m_imp->DeleteBufferData(id);
}

void UserBufferData::GetBufferDataNames(std::vector<std::string>& dst) const
{
    m_imp->GetBufferDataNames(dst);
}