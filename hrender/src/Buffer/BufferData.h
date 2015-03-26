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
protected:
	int m_type;
public:
	enum {
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
    BufferData() { m_type = TYPE_NONE; }
    BufferData(int t) { m_type = t; }
	~BufferData(){}
	int GetType() { return m_type; }
};

#endif //_BUFFERDATA_H_
