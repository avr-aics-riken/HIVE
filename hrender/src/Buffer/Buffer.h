#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Ref.h"
#include <vector>


enum BufferType{
   	BUFFER_VEC2,
	BUFFER_VEC3,
	BUFFER_VEC4,
	BUFFER_FLOAT,
	BUFFER_INDEX
};

class BaseBuffer : public RefCount
{
public:
	virtual BufferType GetType() const = 0;
	BaseBuffer() : RefCount(){}
	virtual ~BaseBuffer(){}
};

class Vec2Buffer : public BaseBuffer
{
public:
    Vec2Buffer() : BaseBuffer() {}
    ~Vec2Buffer(){}
    BufferType GetType() const { return BUFFER_VEC2; }
    
    int Create(int num);
    int GetNum();
    float* GetBuffer();
    
private:
    struct vec2{ float x,y; };
    std::vector<vec2> m_buffer;
};

class Vec3Buffer : public BaseBuffer
{
public:
	Vec3Buffer() : BaseBuffer() {}
	~Vec3Buffer(){}
	BufferType GetType() const { return BUFFER_VEC3; }
	
	int Create(int num);
	int GetNum();
	float* GetBuffer();
	
private:
	struct vec3{ float x,y,z; };
	std::vector<vec3> m_buffer;
};

class Vec4Buffer : public BaseBuffer
{
public:
	Vec4Buffer() : BaseBuffer() {}
	~Vec4Buffer(){}
	BufferType GetType() const { return BUFFER_VEC4; }
	
	int Create(int num);
	int GetNum();
	float* GetBuffer();
	int Set(int i, float x, float y, float z, float w);
	
private:
	struct vec4{ float x,y,z,w; };
	std::vector<vec4> m_buffer;
};

class FloatBuffer : public BaseBuffer
{
public:
	FloatBuffer() : BaseBuffer() {}
	~FloatBuffer(){}
	BufferType GetType() const { return BUFFER_FLOAT; }
	
	int Create(int num);
	int GetNum();
	float* GetBuffer();
	
protected:
	std::vector<float> m_buffer;
};

class UintBuffer : public BaseBuffer
{
public:
	UintBuffer() : BaseBuffer() {}
	~UintBuffer(){}
	BufferType GetType() const { return BUFFER_INDEX; }
	
	int Create(int num);
	int GetNum();
	unsigned int* GetBuffer();
	
private:
	std::vector<unsigned int> m_buffer;
};

#endif //_BUFFER_H_

