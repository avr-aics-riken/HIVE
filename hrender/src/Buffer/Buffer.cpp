#include "Buffer.h"



int Vec3Buffer::Create(int num)
{
	m_buffer.resize(num);
	return num;
}
int Vec3Buffer::GetNum()
{
	return static_cast<int>(m_buffer.size());
}
float* Vec3Buffer::GetBuffer()
{
	if (m_buffer.empty())
		return 0;
	return &m_buffer[0].x;
}

//------------------------------------------------------------------------------

int Vec2Buffer::Create(int num)
{
    m_buffer.resize(num);
    return num;
}
int Vec2Buffer::GetNum()
{
    return static_cast<int>(m_buffer.size());
}
float* Vec2Buffer::GetBuffer()
{
    if (m_buffer.empty())
        return 0;
    return &m_buffer[0].x;
}

//------------------------------------------------------------------------------

int Vec4Buffer::Create(int num)
{
	m_buffer.resize(num);
	return num;
}
int Vec4Buffer::GetNum()
{
	return static_cast<int>(m_buffer.size());
}
float* Vec4Buffer::GetBuffer()
{
	if (m_buffer.empty())
		return 0;
	return &m_buffer[0].x;
}

int Vec4Buffer::Set(int i, float x, float y, float z, float w)
{
	if (m_buffer.empty() || m_buffer.size() <= i)
		return 0;
	
	const vec4 v4 = {x,y,z,w};
	m_buffer[i] = v4;
	return 1;
}


//------------------------------------------------------------------------------

int FloatBuffer::Create(int num)
{
	m_buffer.resize(num);
	return num;
}
int FloatBuffer::GetNum()
{
	return static_cast<int>(m_buffer.size());
}
float* FloatBuffer::GetBuffer()
{
	if (m_buffer.empty())
		return 0;
	return &m_buffer[0];
}

//------------------------------------------------------------------------------

int UintBuffer::Create(int num)
{
	m_buffer.resize(num);
	return num;
}
int UintBuffer::GetNum()
{
	return static_cast<int>(m_buffer.size());
}
unsigned int* UintBuffer::GetBuffer()
{
	if (m_buffer.empty())
		return 0;
	return &m_buffer[0];
}

