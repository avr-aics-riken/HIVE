#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "LuaUtil.h"
#include "Math.h"
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
    
public:
    // Script Bind
    LUA_SCRIPTCLASS_BEGIN(Vec2Buffer)
    LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,int)
    LUA_SCRIPTCLASS_METHOD_ARG0(int,GetNum)
    LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Vec2Buffer);


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
	
public:
	// Script Bind
	LUA_SCRIPTCLASS_BEGIN(Vec3Buffer)
	LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,GetNum)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Vec3Buffer);

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
	
public:
	// Script Bind
	LUA_SCRIPTCLASS_BEGIN(Vec4Buffer)
	LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,GetNum)
	LUA_SCRIPTCLASS_METHOD_ARG5(int,Set,int,float,float,float,float)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(Vec4Buffer);

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
	
public:
	// Script Bind
	LUA_SCRIPTCLASS_BEGIN(FloatBuffer)
	LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,GetNum)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(FloatBuffer);

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
	
public:
	// Script Bind
	LUA_SCRIPTCLASS_BEGIN(UintBuffer)
	LUA_SCRIPTCLASS_METHOD_ARG1(int,Create,int)
	LUA_SCRIPTCLASS_METHOD_ARG0(int,GetNum)
	LUA_SCRIPTCLASS_END()
};
LUA_SCRIPTCLASS_CAST_AND_PUSH(UintBuffer);

#endif //_BUFFER_H_
