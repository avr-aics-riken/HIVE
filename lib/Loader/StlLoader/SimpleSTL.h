/**
 * @file SimpleSTL.h
 * STL形式ファイル簡易ローダー
 */
#ifndef INCLUDE_SIMPLESTL
#define INCLUDE_SIMPLESTL

/*
    SimpleSTL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
#define begin_digit(c) (((c) >= '0' && (c) <= '9') || (c)=='-' || (c)=='+')

inline size_t step_fatof (const char *p, float* val)
{
    const char* start_p = p;
    int frac=0;
    float sign, value, scale;
    
    // skip white space
    while (white_space(*p))
        p += 1;
    
    // sign
    sign = 1.0f;
    if (*p == '-') {
        sign = -1.0f;
        p += 1;
    } else if (*p == '+') {
        p += 1;
    }
    
    // Get digits before decimal point or exponent, if any.
    value = 0.0f;
    while (valid_digit(*p)) {
        value = value * 10.0f + (*p - '0');
        p += 1;
    }
    
    // Get digits after decimal point, if any.
    
    if (*p == '.') {
        float pow10 = 10.0f;
        p += 1;
        
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0f;
            p += 1;
        }
    }
    
    // Handle exponent, if any.
    
    scale = 1.0f;
    if ((*p == 'e') || (*p == 'E')) {
        unsigned int expon;
        p += 1;
        
        // Get sign of exponent, if any.
        frac = 0;
        if (*p == '-') {
            frac = 1;
            p += 1;
        } else if (*p == '+') {
            p += 1;
        }
        
        // Get digits of exponent, if any.
        expon = 0;
        while (valid_digit(*p)) {
            expon = expon * 10 + (*p - '0');
            p += 1;
        }
        if (expon > 38) expon = 38;
        
        // Calculate scaling factor.
        
        while (expon >= 16) { scale *= 1E16f; expon -= 16; }
        while (expon >=  8) { scale *= 1E8f;  expon -=  8; }
        while (expon >   0) { scale *= 10.0f; expon -=  1; }
    }
    
    // Return signed and scaled floating point result.
    
    *val = sign * (frac ? (value / scale) : (value * scale));
    return static_cast<size_t>(p - start_p);
}

inline int searchReturn(const char* a)
{
    int i = 0;
    while(a[i] != 0x0A) i++; // LF
    return ++i;
}
inline int searchSpace(const char* a)
{
    int i = 0;
    while(a[i] != 0x20 && a[i] != 0x09) ++i; // SPACE, TAB
    return ++i;
}
inline int skipSpace(const char* a)
{
    int i = 0;
    while(a[i] == 0x20 || a[i] == 0x09) ++i; // SPACE, TAB
    return i;
}

inline int searchSpaceReturn(const char* a)
{
    int i = 0;
    while(a[i] != 0x20 && a[i] != 0x09 && a[i] != 0x0A) ++i; // SPACE,TAB  or LF
    return i;
}

/**
 * SimpleSTL
 */
class SimpleSTL
{
public:
	SimpleSTL(){}
	virtual ~SimpleSTL(){}
	
    float* GetPositionBuffer()
    {
        return &m_pos[0].x;
    }
    float* GetNormalBuffer()
    {
        return &m_normal[0].x;
    }
    
    unsigned int* GetIndex()
    {
		if (m_face.size())
			return &m_face[0].i0;
		else
			return 0;
    }
	
    unsigned int GetVertexNum() const
    {
        return static_cast<unsigned int>(m_pos.size());
    }
    
    unsigned int GetIndexNum() const
    {
        return static_cast<unsigned int>(m_face.size() * 3);
    }

    /**
     * SimpleSTLA Triangle Info
     */
	struct triData
	{
		float normal[3];
		float vtx0[3];
		float vtx1[3];
		float vtx2[3];
		unsigned short pad;
	};
    
    virtual bool Load(const char* filename, bool generateIndex = false) = 0;
	
protected:
	struct vec3{
        float x,y,z;
		vec3(){}
		vec3(float x_, float y_, float z_) : x(x_),y(y_),z(z_){}
        
		vec3 operator-(const vec3& b) const
        {
            return vec3(this->x - b.x, this->y - b.y, this->z - b.z);
        }
        vec3 operator+(const vec3& b) const
        {
            return vec3(this->x + b.x, this->y + b.y, this->z + b.z);
        }
    };
    vec3 cross(const vec3& a, const vec3& b) const
    {
        return vec3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }
    float length(const vec3& v) const
    {
        return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }
    vec3 normalize(const vec3& v) const
    {
        const float l = length(v);
        return vec3(v.x / l, v.y / l, v.z / l);
    }
	
	struct Index{
        unsigned int i0,i1,i2;
    };
	std::vector<vec3>  m_pos;
    std::vector<vec3>  m_normal;
    std::vector<Index> m_face;

};

#endif // INCLUDE_SIMPLESTL

