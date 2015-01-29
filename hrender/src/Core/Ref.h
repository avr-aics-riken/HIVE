
#pragma once
/*
    Reference count
*/

#include <assert.h>

class RefCount
{
protected:
    RefCount()         { m_ref = 0; }
    virtual ~RefCount(){};
    void* m_userData;
public:
    int Ref()          { return ++m_ref; }
    int GetRef() const { return m_ref; }
    int Unref()        {
        const int ref = --m_ref;
        assert(ref >= 0);
        if (!ref)
            delete this;
        return ref;
    }
    void SetUserData(void* userData) { m_userData = userData; }
private:
    int m_ref;
};

template<class T>
class RefPtr
{
public:
    RefPtr()
    {
        m_p = 0;
    }
    RefPtr(T* s)
    {
        m_p = s;
        if (m_p)
            m_p->Ref();
    }
    RefPtr(const RefPtr& p)
    {
        m_p = p;
        if (m_p)
            m_p->Ref();
    }
    ~RefPtr()
    {
        if (m_p)
            m_p->Unref();
    }
    
    T* Get() const
    {
        return m_p;
    }
    
    operator T*() const
    {
        return m_p;
    }
    T& operator *() const
    {
        assert(m_p);
        return *m_p;
    }
    T* operator->() const
    {
        assert(m_p);
        return m_p;
    }
    bool operator !() const
    {
        return m_p == 0;
    }
    bool operator ==(T* p) const
    {
        return m_p == p;
    }
    bool operator !=(T* p) const
    {
        return m_p != p;
    }
    bool operator <(T* p) const // for STL container
    {
        return m_p < p;
    }
    
    //equal operator
    T* operator =(T* p)
    {
        if (p)
            p->Ref();
        if (m_p)
            m_p->Unref();
        m_p = p;
        return m_p;
    }
    const RefPtr& operator =(const RefPtr& r)
    {
        if (this->m_p == r.m_p)
            return *this;
        
        if (r.m_p)
            r.m_p->Ref();
        if (m_p)
            m_p->Unref();
        m_p = r.m_p;
        
        return *this;
    }
    
private:
    T* m_p;
};

