#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdio.h>
#include "Ref.h"
#include <vector>


enum BufferType{
   	BUFFER_VEC2,
	BUFFER_VEC3,
	BUFFER_VEC4,
	BUFFER_FLOAT,
	BUFFER_INDEX,
    BUFFER_BYTE
};

class BaseArrayBuffer : public RefCount
{
public:
	virtual BufferType GetType() const = 0;
	BaseArrayBuffer() : RefCount(){}
	virtual ~BaseArrayBuffer(){}
};

class Vec2Buffer : public BaseArrayBuffer
{
public:
    Vec2Buffer() : BaseArrayBuffer() {}
    ~Vec2Buffer(){ m_buffer.clear(); std::vector<vec2>(m_buffer).swap(m_buffer); }
    BufferType GetType() const { return BUFFER_VEC2; }
    
    int Create(int num);
    int GetNum() const;
    float* GetBuffer();
    const float* GetBuffer() const;
    
private:
    struct vec2{ float x,y; };
    std::vector<vec2> m_buffer;
};

class Vec3Buffer : public BaseArrayBuffer
{
public:
	Vec3Buffer() : BaseArrayBuffer() {}
    ~Vec3Buffer(){ m_buffer.clear(); std::vector<vec3>(m_buffer).swap(m_buffer); }
	BufferType GetType() const { return BUFFER_VEC3; }
	
	int Create(int num);
	int GetNum() const;
    float* GetBuffer();
    const float* GetBuffer() const;
    
private:
	struct vec3{ float x,y,z; };
	std::vector<vec3> m_buffer;
};

class Vec4Buffer : public BaseArrayBuffer
{
public:
	Vec4Buffer() : BaseArrayBuffer() {}
	~Vec4Buffer(){ m_buffer.clear(); std::vector<vec4>(m_buffer).swap(m_buffer); }
	BufferType GetType() const { return BUFFER_VEC4; }
	
	int Create(int num);
	int GetNum() const;
    float* GetBuffer();
    const float* GetBuffer() const;
	int Set(int i, float x, float y, float z, float w);
	
private:
	struct vec4{ float x,y,z,w; };
	std::vector<vec4> m_buffer;
};

class FloatBuffer : public BaseArrayBuffer
{
public:
	FloatBuffer() : BaseArrayBuffer() {}
    ~FloatBuffer(){ m_buffer.clear(); std::vector<float>(m_buffer).swap(m_buffer); }
	BufferType GetType() const { return BUFFER_FLOAT; }
	
	int Create(int num);
	int GetNum() const;
	float* GetBuffer();
    const float* GetBuffer() const;

protected:
	std::vector<float> m_buffer;
};

class UintBuffer : public BaseArrayBuffer
{
public:
	UintBuffer() : BaseArrayBuffer() {}
	~UintBuffer(){ m_buffer.clear(); std::vector<unsigned int>(m_buffer).swap(m_buffer); }
	BufferType GetType() const { return BUFFER_INDEX; }
	
	int Create(int num);
	int GetNum() const;
    unsigned int* GetBuffer();
    const unsigned int* GetBuffer() const;
	
private:
	std::vector<unsigned int> m_buffer;
};

class ByteBuffer : public BaseArrayBuffer
{
public:
    ByteBuffer() : BaseArrayBuffer() {}
    ~ByteBuffer(){ m_buffer.clear(); std::vector<unsigned char>(m_buffer).swap(m_buffer); }
    BufferType GetType() const { return BUFFER_BYTE; }
    
    int Create(int num);
    int GetNum() const;
    unsigned char* GetBuffer();
    const unsigned char* GetBuffer() const;
    
private:
    std::vector<unsigned char> m_buffer;
};

#endif //_BUFFER_H_

