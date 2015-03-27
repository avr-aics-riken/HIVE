/**
 * @file BufferData.h
 * BufferDataクラス
 */
#ifndef _BUFFERDATA_H_
#define _BUFFERDATA_H_

#include "Ref.h"

/**
 * BufferDataクラス
 */
class BufferData : public RefCount
{
private:
	int m_type;
    mutable bool m_needupdate;
public:
	enum BufferType{
		TYPE_NONE = 0,
		TYPE_MESH,
		TYPE_POINT,
		TYPE_LINE,
		TYPE_SPHERE,
		TYPE_VECTOR,
		TYPE_TETRA,
		TYPE_VOLUME,
		TYPE_SVOLUME,
		TYPE_IMAGE,
		TYPE_MAX,
	};
    BufferData(BufferType type) : m_type(type), m_needupdate(true) {}
	~BufferData(){}
	int  GetType()      const { return m_type;       }
    void NeedUpdate()         { m_needupdate = true; }
    bool IsNeedUpdate() const { return m_needupdate; }
    
    // Only use BaseBuffer
    void updated()      const { m_needupdate = false; }
};

#endif //_BUFFERDATA_H_
