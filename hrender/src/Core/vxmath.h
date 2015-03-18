/**
 * @file Math.h
 * 数学関数
 */

#ifndef INCLUDE_VX_MATH_H
#define INCLUDE_VX_MATH_H

#include <math.h>

namespace VX {
namespace Math {

static const float PI = 3.141592654f;
		
inline float Floor(float x)
{
	return floorf(x);
}

inline float Ceil(float x)
{
	return ceilf(x);
}

inline float Mod(float x, float y)
{
	return fmodf(x, y);
}

inline float Log(float x)
{
	return logf(x);
}
	
inline float Sin(float rad)
{
	return sinf(rad);
}

inline float Cos(float rad)
{
	return cosf(rad);
}

inline float Tan(float rad)
{
	return tanf(rad);
}

inline float Asin(float x)
{
	return asinf(x);
}

inline float Acos(float x)
{
	return acosf(x);
}
	
inline float Atan(float x)
{
	return atanf(x);
}

inline float Atan2(float y, float x)
{
	return atan2f(y, x);
}

inline float Sqrt(float s)
{
	return sqrtf(s);
}
	
inline float Pow(float x, float y)
{
	return powf(x,y);
}

inline float Abs(float s) 
{
	return (s >= 0.0f) ? s : -s;
}
	
inline float Clamp(float x, float min_v, float max_v) 
{
	return (x <= min_v) ? min_v : ((x >= max_v) ? max_v : x);
}

inline float ToRadian(float degree )
{
	return degree * (PI / 180.0f);		
}
	
inline float ToDegree(float radian)
{
	return radian * (180.0f / PI);
}

#ifndef _WINDOWS_ 
template<typename T>
inline T max(const T& a, const T& b)
{
	return (a < b) ? b : a;
}

template<typename T>
inline T max(const T& a, const T& b, const T& c)
{
	return max(max(a,b),c);
}
	
template<typename T>
inline T min(const T& a, const T& b)
{
	return (a < b) ? a : b;
}
	
template<typename T>
inline T min(const T& a, const T& b, const T& c)
{
	return min(min(a,b),c);
}
#endif
	
//=================================================
	
#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
#define begin_digit(c) (((c) >= '0' && (c) <= '9') || (c)=='-' || (c)=='+')
	
/*	
inline u32 step_atoi (const char *p, s32* val)
{
	const char* start_p = p;
	s32 sign = 1, value = 0;
	
	// skip white space
	while (white_space(*p))
		p += 1;
	
	// sign
	if (*p == '-') {
		sign = -1;
		p += 1;
	} else if (*p == '+') {
		p += 1;
	}
	
	while (valid_digit(*p)) {
		value = value * 10 + (*p - '0');
		p += 1;
	}
	
	*val = value;// * sign;
	return p - start_p;
}*/

inline unsigned int step_fatof (const char *p, float* val)
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
	return static_cast<unsigned int>(p - start_p);
}

// =================  Vector, matrix, Quartanion  ====================

/// 2次元ベクトルユーティリティ
struct vec2
{
	float x,y;
	
	vec2()                                        {/* to do nothing. */ }
	vec2(float x_, float y_)                      { x = x_; y = y_;     }
	vec2(const float* const v)                    { x = v[0]; y = v[1]; }
	vec2 operator-() const                        { return vec2(-x, -y);}
	float & operator[](int i)                     { return (&x)[i];     }
	float const & operator[](int i) const         { return (&x)[i];     }
	vec2& operator+=(const vec2& a) { x += a.x; y += a.y; return *this; }
	vec2& operator-=(const vec2& a) { x -= a.x; y -= a.y; return *this; }
	vec2& operator*=(const vec2& a) { x *= a.x; y *= a.y; return *this; }
	vec2& operator/=(const vec2& a) { x /= a.x; y /= a.y; return *this; }
	vec2& operator+=(float s)       { x += s;   y += s;   return *this; }
	vec2& operator-=(float s)       { x -= s;   y -= s;   return *this; }
	vec2& operator*=(float s)       { x *= s;   y *= s;   return *this; }
	vec2& operator/=(float s)       { const float inv = 1.0f/s;
		x *= inv;   y *= inv;   return *this; }
};

/// 3次元ベクトルユーティリティ
struct vec3
{
	float x,y,z;
	
	vec3()                                        {     /* to do nothing. */      }
	vec3(const vec2& v, float z_ = 0)             { x = v.x; y = v.y; z = z_;     }
	vec3(float x_, float y_, float z_)            { x = x_; y = y_; z = z_;       }
	vec3(const float* const v)                    { x = v[0]; y = v[1]; z = v[2]; }
	vec3 operator-() const                        { return vec3(-x, -y, -z);      }
	float & operator[](int i)                     { return (&x)[i];               }
    float const & operator[](int i) const         { return (&x)[i];               }
	vec3& operator+=(const vec3& a) { x += a.x; y += a.y; z += a.z; return *this; }
    vec3& operator-=(const vec3& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
	vec3& operator*=(const vec3& a) { x *= a.x; y *= a.y; z *= a.z; return *this; }
    vec3& operator/=(const vec3& a) { x /= a.x; y /= a.y; z /= a.z; return *this; }
    vec3& operator+=(float s)       { x += s;   y += s;   z += s;   return *this; }
    vec3& operator-=(float s)       { x -= s;   y -= s;   z -= s;   return *this; }
	vec3& operator*=(float s)       { x *= s;   y *= s;   z *= s;   return *this; }
    vec3& operator/=(float s)       { const float inv = 1.0f/s;
		x *= inv;   y *= inv;   z *= inv;   return *this; }
	vec2 xy() const                 { return vec2(x, y); }
	vec2 yx() const                 { return vec2(y, x); }
	vec2 xz() const                 { return vec2(x, z); }
	vec2 zx() const                 { return vec2(z, x); }
	vec2 yz() const                 { return vec2(y, z); }
	vec2 zy() const                 { return vec2(z, y); }
	vec2 xx() const                 { return vec2(x, x); }
	vec2 yy() const                 { return vec2(y, y); }
	vec2 zz() const                 { return vec2(z, z); }
};
	
/// 4次元ベクトルユーティリティ
struct vec4
{
	float x,y,z,w;
	
	vec4()                                        {     /* to do nothing. */                }
	vec4(const vec2& v, float z_=0, float w_=0)   { x = v.x; y = v.y; z = z_; w = w_;       }
	vec4(const vec3& v, float w_=0)               { x = v.x; y = v.y; z = v.z; w = w_;      }
	vec4(float x_, float y_, float z_, float w_)  { x = x_; y = y_; z = z_; w = w_;         }
	vec4(const float* const v)                    { x = v[0]; y = v[1]; z = v[2]; w = v[3]; }
	vec4 operator-() const                        { return vec4(-x, -y, -z, -w);            }
	float & operator[](int i)                     { return (&x)[i];                         }
	float const & operator[](int i) const         { return (&x)[i];                         }
	vec4& operator+=(const vec4& a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
	vec4& operator-=(const vec4& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this; }
	vec4& operator*=(const vec4& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; return *this; }
	vec4& operator/=(const vec4& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this; }
	vec4& operator+=(float s)       { x += s;   y += s;   z += s;   w += s;   return *this; }
	vec4& operator-=(float s)       { x -= s;   y -= s;   z -= s;   w -= s;   return *this; }
	vec4& operator*=(float s)       { x *= s;   y *= s;   z *= s;   w *= s;   return *this; }
	vec4& operator/=(float s)       { const float inv = 1.0f/s; 
		x *= inv;   y *= inv;   z *= inv;   w *= inv;   return *this; }
	vec3 xyz() const                { return vec3(x, y, z); }
	vec3 zyx() const                { return vec3(z, y, x); }
	vec3 xxx() const                { return vec3(x, x, x); }
	vec3 yyy() const                { return vec3(y, y, y); }
	vec3 zzz() const                { return vec3(z, z, z); }
	vec3 www() const                { return vec3(w, w, w); }
	vec2 xy () const                { return vec2(x, y);    }
	vec2 yz () const                { return vec2(y, z);    }
	vec2 xz () const                { return vec2(x, z);    }
	vec2 yx () const                { return vec2(y, x);    }
	vec2 zy () const                { return vec2(x, y);    }
	vec2 zx () const                { return vec2(z, x);    }
	vec2 xx () const                { return vec2(x, x);    }
	vec2 yy () const                { return vec2(y, y);    }
	vec2 zz () const                { return vec2(z, z);    }
};

	
struct matrix3x3;
struct matrix2x2;
	
/// 4x4行列ユーティリティ
struct matrix4x4
{
	union
	{
		float m[4][4];
		float f[16];
	};
	matrix4x4() {}
	matrix4x4(const float* const v) { for (int i = 0; i < 16; i++) f[i] = v[i]; };
	matrix4x4(const float m00, const float m01, const float m02, const float m03,
			  const float m10, const float m11, const float m12, const float m13,
			  const float m20, const float m21, const float m22, const float m23,
  			  const float m30, const float m31, const float m32, const float m33)
	{
		f[ 0] = m00; f[ 1] = m01; f[ 2] = m02; f[ 3] = m03;
		f[ 4] = m10; f[ 5] = m11; f[ 6] = m12; f[ 7] = m13;
		f[ 8] = m20; f[ 9] = m21; f[10] = m22; f[11] = m23;
		f[12] = m30; f[13] = m31; f[14] = m32; f[15] = m33;
	};

	matrix4x4(const matrix3x3& a)
	{
		const float* x = reinterpret_cast<const float*>(&a); // force cast
		f[0] = x[0]; f[1] = x[1]; f[ 2] = x[2];
		f[4] = x[3]; f[5] = x[4]; f[ 6] = x[5];
		f[8] = x[6]; f[9] = x[7]; f[10] = x[8];
	}
	matrix4x4(const vec4& a, const vec4& b, const vec4& c, const vec4& d)
	{
		f[ 0] = a.x; f[ 1] = a.y; f[ 2] = a.z; f[ 3] = a.w;
		f[ 4] = b.x; f[ 5] = b.y; f[ 6] = b.z; f[ 7] = b.w; 
		f[ 8] = c.x; f[ 9] = c.y; f[10] = c.z; f[11] = c.w;
		f[12] = d.x; f[13] = d.y; f[14] = d.z; f[15] = d.w;
	}
	/*
	bool operator!=(const matrix4x4& t)
	{
		if (f[ 0] != t.f[ 0] || f[ 1] != t.f[ 1] || f[ 2] != t.f[ 2] || f[ 3] != t.f[ 3] ||
			f[ 4] != t.f[ 4] || f[ 5] != t.f[ 5] || f[ 6] != t.f[ 6] || f[ 7] != t.f[ 7] ||
			f[ 8] != t.f[ 8] || f[ 9] != t.f[ 9] || f[10] != t.f[10] || f[11] != t.f[11] ||
			f[12] != t.f[12] || f[13] != t.f[13] || f[14] != t.f[14] || f[15] != t.f[15])
			return true;
		else
			return false;
	}*/
};
	
/// 3x3行列ユーティリティ
struct matrix3x3
{
	union
	{
		float m[3][3];
		float f[9];
	};
	matrix3x3() {}
	matrix3x3(const float* const v) { for (int i = 0; i < 9; i++) f[i] = v[i]; };
	matrix3x3(const float m00, const float m01, const float m02,
			  const float m10, const float m11, const float m12,
			  const float m20, const float m21, const float m22)
	{
		f[0] = m00; f[1] = m01;	f[2] = m02;
		f[3] = m10; f[4] = m11;	f[5] = m12;
		f[6] = m20; f[7] = m21;	f[8] = m22;
	};
	matrix3x3(const vec3& a, const vec3& b, const vec3& c)
	{
		f[0] = a.x; f[1] = a.y; f[2] = a.z; 
		f[3] = b.x; f[4] = b.y; f[5] = b.z; 
		f[6] = c.x; f[7] = c.y; f[8] = c.z; 
	}
	matrix3x3 (const matrix4x4& a)
	{
		f[0] = a.m[0][0]; f[1] = a.m[0][1]; f[2] = a.m[0][2];
		f[3] = a.m[1][0]; f[4] = a.m[1][1]; f[5] = a.m[1][2];
		f[6] = a.m[2][0]; f[7] = a.m[2][1]; f[8] = a.m[2][2];
	}
};

/// 2x2行列ユーティリティ
struct matrix2x2
{
	union
	{
		float m[2][2];
		float f[4];
	};
	matrix2x2() {}
	matrix2x2(const float* const v) { for (int i = 0; i < 4; i++) f[i] = v[i]; };
	matrix2x2(const float m00, const float m01,
			  const float m10, const float m11)
	{
		f[0] = m00; f[1] = m01;
		f[2] = m10; f[2] = m11;
	};

	matrix2x2(const vec2& a, const vec2& b)
	{
		f[0] = a.x; f[1] = a.y; 
		f[2] = b.x; f[3] = b.y;
	}
	matrix2x2(const matrix4x4& a)
	{
		f[0] = a.m[0][0]; f[1] = a.m[0][1];
		f[2] = a.m[1][0]; f[3] = a.m[1][1];
	}
	matrix2x2(const matrix3x3& a)
	{
		f[0] = a.m[0][0]; f[1] = a.m[0][1];
		f[2] = a.m[1][0]; f[3] = a.m[1][1];
	}
};

/// 四元数ユーティリティ
struct quaternion
{
	float x,y,z,w;
	
	quaternion() {}
    quaternion(const float* const v)
	{
		x = v[0]; y = v[1]; z = v[2]; w = v[3];
	}
    quaternion( float x_, float y_, float z_, float w_ )
	{
		x = x_; y = y_; z = z_; w = w_;
	}
	float& operator [](int i)             { return (&x)[i]; }
	const float& operator [](int i) const { return (&x)[i]; }
	quaternion operator-() const                        { return quaternion(-x, -y, -z, -w);            }
	quaternion& operator+=(const quaternion& a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
	quaternion& operator-=(const quaternion& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this; }
	quaternion& operator+=(float s)             { x += s;   y += s;   z += s;   w += s;   return *this; }
	quaternion& operator-=(float s)             { x -= s;   y -= s;   z -= s;   w -= s;   return *this; }
	quaternion& operator*=(float s)             { x *= s;   y *= s;   z *= s;   w *= s;   return *this; }
	quaternion& operator/=(float s) { const float inv = 1.0f / s;
		x *= inv;   y *= inv;   z *= inv;   w *= inv;   return *this; }
};

typedef matrix4x4 matrix;
typedef matrix4x4 mat4;
typedef matrix3x3 mat3;
typedef matrix2x2 mat2;
	
// ----------------------- vec4 --------------------------------

inline float length(const vec4& v)
{
	return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec4 normalize(const vec4& v)
{
	const float l = length(v);
	return vec4(v.x / l, v.y / l, v.z / l, v.w / l);
}

inline vec4 operator+(const vec4& a, const vec4& b)
{
	return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline vec4 operator-(const vec4& a, const vec4& b)
{
	return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w + b.w);
}

inline vec4 operator+(const vec4& a, const float r)
{
	return vec4(a.x + r, a.y + r, a.z + r, a.w + r);
}

inline vec4 operator+(const float r, const vec4& a)
{
	return vec4(a.x + r, a.y + r, a.z + r, a.w + r);
}

inline vec4 operator-(const vec4& a, const float r)
{
	return vec4(a.x - r, a.y - r, a.z - r, a.w - r);
}

inline vec4 operator-(const float r, const vec4& a)
{
	return vec4(r - a.x, r - a.y, r - a.z, r - a.w); 
}

inline vec4 operator*(const vec4& a, const float r)
{
	return vec4(a.x * r, a.y * r, a.z * r, a.w * r);
}

inline vec4 operator*(const float r, const vec4& a)
{
	return vec4(a.x * r, a.y * r, a.z * r, a.w * r);
}

inline vec4 operator/(const vec4& a, const float r)
{
	const float inv = 1.0f / r;
	return vec4(a.x * inv, a.y * inv, a.z * inv, a.w * inv);
}

inline vec4 operator/(const float r, const vec4& a)
{
	return vec4(r / a.x, r / a.y, r / a.z, r /a.w);
}

inline float dot(const vec4& a, const vec4& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}
	
inline vec4 cross(const vec4& a, const vec4& b, const vec4& c)
{
	return vec4(
			a.y * (b.z * c.w - c.z * b.w) - a.z * (b.y * c.w - c.y * b.w) + a.w * (b.y * c.z - b.z *c.y),
			-(a.x * (b.z * c.w - c.z * b.w) - a.z * (b.x * c.w - c.x * b.w) + a.w * (b.x * c.z - c.x * b.z)),
			  a.x * (b.y * c.w - c.y * b.w) - a.y * (b.x * c.w - c.x * b.w) + a.w * (b.x * c.y - c.x * b.y),
			-(a.x * (b.y * c.z - c.y * b.z) - a.y * (b.x * c.z - c.x * b.z) + a.z * (b.x * c.y - c.x * b.y)));
}
	
inline vec4 lerp(const vec4& a, const vec4& b, const float r)
{
	return vec4((b - a) * r + a);
}
	
inline bool operator==(const vec4& a, const vec4& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline bool operator!=(const vec4& a, const vec4& b)
{
	return (a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w);
}

inline vec4 Maximize(const vec4& a, const vec4& b)
{
	return vec4(a.x < b.x ? b.x : a.x,
				a.y < b.y ? b.y : a.y,
				a.z < b.z ? b.z : a.z,
				a.w < b.w ? b.w : a.w);
}

inline vec4 Minimize(const vec4& a, const vec4& b)
{
	return vec4(a.x < b.x ? a.x : b.x,
				a.y < b.y ? a.y : b.y,
				a.z < b.z ? b.z : a.z,
				a.w < b.w ? b.w : a.w);
}

	
// ----------------------- vec3 --------------------------------
	
inline float length(const vec3& v)
{
	return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline vec3 normalize(const vec3& v)
{
	const float l = length(v);
	return vec3(v.x / l, v.y / l, v.z / l);
}

inline vec3 operator+(const vec3& a, const vec3& b)
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator+(const vec3& a, const float r)
{
	return vec3(a.x + r, a.y + r, a.z + r);
}

inline vec3 operator+(const float r, const vec3& a)
{
	return vec3(a.x + r, a.y + r, a.z + r);
}

inline vec3 operator-(const vec3& a, const float r)
{
	return vec3(a.x - r, a.y - r, a.z - r);
}

inline vec3 operator-(const float r, const vec3& a)
{
	return vec3(r - a.x, r - a.y, r - a.z);
}

inline vec3 operator*(const vec3& a, const float r)
{
	return vec3(a.x * r, a.y * r, a.z * r);
}

inline vec3 operator*(const float r, const vec3& a)
{
	return vec3(a.x * r, a.y * r, a.z * r);
}
	
inline vec3 operator/(const vec3& a, const vec3& b)
{
	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}
	
inline vec3 operator*(const vec3& a, const vec3& b)
{
	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}


inline vec3 operator/(const vec3& a, const float r)
{
	const float inv = 1.0f / r;
	return vec3(a.x * inv, a.y * inv, a.z * inv);
}

inline vec3 operator/(const float r, const vec3& a)
{
	return vec3(r / a.x, r / a.y, r / a.z);
}
	
inline float dot(const vec3& a, const vec3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
}

inline vec3 lerp(const vec3& a, const vec3& b, const float r)
{
	return vec3((b - a) * r + a);
}

inline bool operator==(const vec3& a, const vec3& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool operator!=(const vec3& a, const vec3& b)
{
	return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
}
	
inline vec3 Maximize(const vec3& a, const vec3& b)
{
	return vec3(a.x < b.x ? b.x : a.x,
				a.y < b.y ? b.y : a.y,
				a.z < b.z ? b.z : a.z);
}

inline vec3 Minimize(const vec3& a, const vec3& b)
{
	return vec3(a.x < b.x ? a.x : b.x,
				a.y < b.y ? a.y : b.y,
				a.z < b.z ? a.z : b.z);
}

	
// ----------------------- vec2 --------------------------------

inline float length(const vec2& v)
{
	return Sqrt(v.x * v.x + v.y * v.y);
}

inline vec2 normalize(const vec2& v)
{
	const float l = length(v);
	return vec2(v.x / l, v.y / l);
}

inline vec2 operator+(const vec2& a, const vec2& b)
{
	return vec2(a.x + b.x, a.y + b.y);
}

inline vec2 operator-(const vec2& a, const vec2& b)
{
	return vec2(a.x - b.x, a.y - b.y);
}

inline vec2 operator+(const vec2& a, const float r)
{
	return vec2(a.x + r, a.y + r);
}

inline vec2 operator+(const float r, const vec2& a)
{
	return vec2(a.x + r, a.y + r);
}

inline vec2 operator-(const vec2& a, const float r)
{
	return vec2(a.x - r, a.y - r);
}

inline vec2 operator-(const float r, const vec2& a)
{
	return vec2(r - a.x, r - a.y);
}
	
inline vec2 operator*(const vec2& a, const float r)
{
	return vec2(a.x * r, a.y * r);
}

inline vec2 operator*(const float r, const vec2& a)
{
	return vec2(a.x * r, a.y * r);
}

inline vec2 operator/(const vec2& a, const float r)
{
	const float inv = 1.0f / r;
	return vec2(a.x * inv, a.y * inv);
}

inline vec2 operator/(const vec2& a, const vec2& b)
{
	return vec2(a.x / b.x, a.y / b.y);
}

inline vec2 operator*(const vec2& a, const vec2& b)
{
	return vec2(a.x * b.x, a.y * b.y);
}

inline vec2 operator/(const float r, const vec2& a)
{
	return vec2(r / a.x, r / a.y);
}
	
inline float dot(const vec2& a, const vec2& b)
{
	return (a.x * b.x + a.y * b.y);
}

inline vec2 cross(const vec2& a, const vec2& b)
{
	return vec2(a.y * b.x - a.x * b.y,
				a.x * b.y - a.y * b.x);
}

inline vec2 lerp(const vec2& a, const vec2& b, const float r)
{
	return vec2((b - a) * r + a);
}	
	
inline bool operator==(const vec2& a, const vec2& b)
{
	return (a.x == b.x) && (a.y == b.y);
}
	
inline bool operator!=(const vec2& a, const vec2& b)
{
	return (a.x != b.x) || (a.y != b.y);
}

inline vec2 Maximize(const vec2& a, const vec2& b)
{
	return vec2(a.x < b.x ? b.x : a.x,
				a.y < b.y ? b.y : a.y);
}

inline vec2 Minimize(const vec2& a, const vec2& b)
{
	return vec2(a.x < b.x ? a.x : b.x,
				a.y < b.y ? a.y : b.y);
}

// ----------------------- matrix4x4 --------------------------------
		
inline matrix4x4 Identity()
{
	return matrix4x4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
}

inline matrix4x4 Translation(float x, float y, float z)
{
	return matrix4x4(1,0,0,0, 0,1,0,0, 0,0,1,0, x,y,z,1);
}

inline matrix4x4 Translation(vec3 v)
{
	return matrix4x4(1,0,0,0, 0,1,0,0, 0,0,1,0, v.x, v.y, v.z,1);
}

inline matrix4x4 Scaling(float s)
{
	return matrix4x4(s,0,0,0, 0,s,0,0, 0,0,s,0, 0,0,0,1);
}

inline matrix4x4 Scaling(float x, float y, float z = 1.0f)
{
	return matrix4x4(x,0,0,0, 0,y,0,0, 0,0,z,0, 0,0,0,1);
}

inline matrix4x4 RotationX(float degrees)
{
	const float rad = ToRadian(degrees);
	const float s = Sin(rad);
	const float c = Cos(rad);
	
	return matrix4x4(
		1,  0, 0, 0,
		0,  c, s, 0,
		0, -s, c, 0,
		0,  0, 0, 1
	);
}

inline matrix4x4 RotationY(float degrees)
{
	const float rad = ToRadian(degrees);
	const float s = Sin(rad);
	const float c = Cos(rad);
	
	return matrix4x4(
		c, 0, -s, 0,
		0, 1,  0, 0,
		s, 0,  c, 0,
		0, 0,  0, 1
	);
}

inline matrix4x4 RotationZ(float degrees)
{
	const float rad = ToRadian(degrees);
	const float s = Sin(rad);
	const float c = Cos(rad);
	
	return matrix4x4(
		 c, s, 0, 0,
		-s, c, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	);
}
	
inline matrix4x4 RotationAxis(const vec3& v, float degree)
{
	const float rad = ToRadian(degree);
	const float cosa = Cos(rad);
    const float sina = Sin(rad);
    const float x = v.x, y = v.y, z = v.z;
	return matrix4x4(
		cosa + (1.0f - cosa) * x * x,
		(1.0f - cosa) * x * y + sina * z,
		(1.0f - cosa) * x * z - sina * y,
		0,
		(1.0f - cosa) * x * y - z * sina,
		cosa + (1 - cosa) * y * y,
		(1.0f - cosa) * y * z + x * sina,
		0,
		(1.0f - cosa) * x * z + sina * y,
		(1.0f - cosa) * y * z - sina * x,
		 cosa + (1.0f - cosa) * z * z,
		 0.0f,
		 0.0f,
		 0.0f,
		 0.0f,
		 1.0f
	);
}
	
inline matrix4x4 RotationYawPitchRoll(float yaw_degree, float pitch_degree, float roll_degree)
{
	const float yaw   = ToRadian(yaw_degree);
	const float pitch = ToRadian(pitch_degree);
	const float roll  = ToRadian(roll_degree);
	const float sin_y = Sin( yaw );
	const float cos_y = Cos( yaw );
	const float sin_p = Sin( pitch );
	const float cos_p = Cos( pitch );
	const float sin_r = Sin( roll );
	const float cos_r = Cos( roll );
	return matrix4x4(
		cos_y * cos_r + sin_y * sin_p * sin_r,
		sin_r * cos_p,
		cos_r * -sin_y + sin_r * sin_p * cos_y,
		0.0f,
			-sin_r * cos_y + cos_r * sin_p * sin_y,
		cos_r * cos_p,
		sin_r * sin_y + cos_r * sin_p * cos_y,
		0.0f,
			cos_p * sin_y,
		-sin_p,
		cos_p * cos_y,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	);
}

inline matrix4x4 operator* (const matrix4x4& a, const matrix4x4& b)
{
	return matrix4x4(
		a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2] + a.m[3][0] * b.m[0][3],
		a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2] + a.m[3][1] * b.m[0][3],
		a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2] + a.m[3][2] * b.m[0][3],
		a.m[0][3] * b.m[0][0] + a.m[1][3] * b.m[0][1] + a.m[2][3] * b.m[0][2] + a.m[3][3] * b.m[0][3],

		a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2] + a.m[3][0] * b.m[1][3],
		a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2] + a.m[3][1] * b.m[1][3],
		a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2] + a.m[3][2] * b.m[1][3],
		a.m[0][3] * b.m[1][0] + a.m[1][3] * b.m[1][1] + a.m[2][3] * b.m[1][2] + a.m[3][3] * b.m[1][3],

		a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2] + a.m[3][0] * b.m[2][3],
		a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2] + a.m[3][1] * b.m[2][3],
		a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2] + a.m[3][2] * b.m[2][3],
		a.m[0][3] * b.m[2][0] + a.m[1][3] * b.m[2][1] + a.m[2][3] * b.m[2][2] + a.m[3][3] * b.m[2][3],

		a.m[0][0] * b.m[3][0] + a.m[1][0] * b.m[3][1] + a.m[2][0] * b.m[3][2] + a.m[3][0] * b.m[3][3],
		a.m[0][1] * b.m[3][0] + a.m[1][1] * b.m[3][1] + a.m[2][1] * b.m[3][2] + a.m[3][1] * b.m[3][3],
		a.m[0][2] * b.m[3][0] + a.m[1][2] * b.m[3][1] + a.m[2][2] * b.m[3][2] + a.m[3][2] * b.m[3][3],
		a.m[0][3] * b.m[3][0] + a.m[1][3] * b.m[3][1] + a.m[2][3] * b.m[3][2] + a.m[3][3] * b.m[3][3]
					  
	);
}
	
inline matrix3x3 operator* (const matrix3x3& a, const matrix3x3& b)
{
	return matrix3x3(
		a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2],
		a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2],
		a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2],

		a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2],
		a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2],
		a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2],

		a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2],
		a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2],
		a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2]
	);
}

inline matrix2x2 operator* (const matrix2x2& a, const matrix2x2& b)
{
	return matrix2x2(
	    a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1],
	    a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1],
	    a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1],
	    a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1]
	);
}

inline matrix4x4 Transpose (const matrix4x4& a)
{
	return matrix4x4(
		a.m[0][0], a.m[1][0], a.m[2][0], a.m[3][0],
		a.m[0][1], a.m[1][1], a.m[2][1], a.m[3][1],		
		a.m[0][2], a.m[1][2], a.m[2][2], a.m[3][2],
		a.m[0][3], a.m[1][3], a.m[2][3], a.m[3][3]
	);
}

inline matrix3x3 Transpose (const matrix3x3& a)
{
	return matrix3x3(
		a.m[0][0], a.m[1][0], a.m[2][0],
		a.m[0][1], a.m[1][1], a.m[2][1],		
		a.m[0][2], a.m[1][2], a.m[2][2]
	);
}

inline matrix2x2 Transpose (const matrix2x2& a)
{
	return matrix2x2(
		a.m[0][0], a.m[1][0],
		a.m[0][1], a.m[1][1]
	);
}
	
inline vec4 operator* (const matrix4x4& a, const vec4& b)
{
	return vec4(
		a.m[0][0] * b.x + a.m[1][0] * b.y + a.m[2][0] * b.z + a.m[3][0] * b.w,
        a.m[0][1] * b.x + a.m[1][1] * b.y + a.m[2][1] * b.z + a.m[3][1] * b.w,
        a.m[0][2] * b.x + a.m[1][2] * b.y + a.m[2][2] * b.z + a.m[3][2] * b.w,
        a.m[0][3] * b.x + a.m[1][3] * b.y + a.m[2][3] * b.z + a.m[3][3] * b.w
	);
}
	
inline vec3 operator* (const matrix3x3& a, const vec3& b)
{
	return vec3(
		a.m[0][0] * b.x + a.m[1][0] * b.y + a.m[2][0] * b.z,
		a.m[0][1] * b.x + a.m[1][1] * b.y + a.m[2][1] * b.z,
		a.m[0][2] * b.x + a.m[1][2] * b.y + a.m[2][2] * b.z
	);
}

inline vec2 operator* (const matrix2x2& a, const vec2& b)
{
	return vec2(
		a.m[0][0] * b.x + a.m[1][0] * b.y,
		a.m[0][1] * b.x + a.m[1][1] * b.y
	);
}


inline matrix4x4 Ortho(float l, float r, float t, float b, float nearval, float farval)
{
	const float x =  2.0f / (r - l);
	const float y =  2.0f / (t - b);
	const float z =  -2.0f / (farval - nearval);
	const float tx = - (r + l) / (r - l);
	const float ty = - (t + b) / (t - b);
	const float tz = - (farval + nearval) / (farval - nearval);
	return matrix4x4(x, 0, 0, 0,
					 0, y, 0, 0,
					 0, 0, z, 0,
					 tx, ty, tz, 1
	);
}

inline matrix4x4 Frustum(float l, float r, float t, float b, float nearval, float farval)
{
	const float x = 2.0f * nearval / (r - l);
	const float y = 2.0f * nearval / (t - b);
	const float z = (r + l) / (r - l);
	const float w = (t + b) / (t - b);
	const float e = - (farval + nearval) / (farval - nearval);
	const float p = -2.0f * farval * nearval / (farval - nearval);
	return matrix4x4(
		x, 0, 0,  0,
		0, y, 0,  0,
		z, w, e, -1,
		0, 0, p,  0
	);
}
	
inline matrix4x4 PerspectiveFov(float fovy_degree, float Aspect, float zn, float zf)
{
	const float h = 1.0f / Tan(ToRadian(fovy_degree) * 0.5f);
	const float w = h / Aspect;
	return matrix4x4(w,    0.0f,  0.0f,               0.0f,
					 0.0f, h,     0.0f,               0.0f,
					 0.0f, 0.0f, -(zf+zn) / (zf-zn), -1.0f,
					 0.0f, 0.0f, -2.0f*zn*zf/(zf-zn), 0.0f);
/*	return matrix4x4(w,    0.0f,  0.0f,               0.0f,
					 0.0f, h,     0.0f,               0.0f,
					 0.0f, 0.0f, (zf+zn) / (zf-zn), -2.0f*zn*zf/(zf-zn),
					 0.0f, 0.0f, -1.0, 0.0f);*/
}
	
inline matrix4x4 LookAt(const vec3& eye, const vec3& target, const vec3& up)
{
	const vec3 z = normalize(eye - target);
	const vec3 x = normalize(cross(up, z));
	const vec3 y = normalize(cross(z, x));
	
	return matrix4x4(
		 x.x, y.x, z.x, 0,
		 x.y, y.y, z.y, 0,
		 x.z, y.z, z.z, 0,
		-dot(x, eye), -dot(y, eye), -dot(z, eye),  1.0f
	);
}

inline float Det(const matrix4x4& mat)
{
	const float det =
	  mat.m[0][0]*mat.m[1][1]*mat.m[2][2]*mat.m[3][3]
	+ mat.m[0][0]*mat.m[1][2]*mat.m[2][3]*mat.m[3][1]
	+ mat.m[0][0]*mat.m[1][3]*mat.m[2][1]*mat.m[3][2]
	+ mat.m[0][1]*mat.m[1][0]*mat.m[2][3]*mat.m[3][2]
	+ mat.m[0][1]*mat.m[1][2]*mat.m[2][0]*mat.m[3][3]
	+ mat.m[0][1]*mat.m[1][3]*mat.m[2][2]*mat.m[3][0]
	+ mat.m[0][2]*mat.m[1][0]*mat.m[2][1]*mat.m[3][3]
	+ mat.m[0][2]*mat.m[1][1]*mat.m[2][3]*mat.m[3][0]
	+ mat.m[0][2]*mat.m[1][3]*mat.m[2][0]*mat.m[3][1]
	+ mat.m[0][3]*mat.m[1][0]*mat.m[2][2]*mat.m[3][1]
	+ mat.m[0][3]*mat.m[1][1]*mat.m[2][0]*mat.m[3][2]
	+ mat.m[0][3]*mat.m[1][2]*mat.m[2][1]*mat.m[3][0]
	- mat.m[0][0]*mat.m[1][1]*mat.m[2][3]*mat.m[3][2]
	- mat.m[0][0]*mat.m[1][2]*mat.m[2][1]*mat.m[3][3]
	- mat.m[0][0]*mat.m[1][3]*mat.m[2][2]*mat.m[3][1]
	- mat.m[0][1]*mat.m[1][0]*mat.m[2][2]*mat.m[3][3]
	- mat.m[0][1]*mat.m[1][2]*mat.m[2][3]*mat.m[3][0]
	- mat.m[0][1]*mat.m[1][3]*mat.m[2][0]*mat.m[3][2]
	- mat.m[0][2]*mat.m[1][0]*mat.m[2][3]*mat.m[3][1]
	- mat.m[0][2]*mat.m[1][1]*mat.m[2][0]*mat.m[3][3]
	- mat.m[0][2]*mat.m[1][3]*mat.m[2][1]*mat.m[3][0]
	- mat.m[0][3]*mat.m[1][0]*mat.m[2][1]*mat.m[3][2]
	- mat.m[0][3]*mat.m[1][1]*mat.m[2][2]*mat.m[3][0]
	- mat.m[0][3]*mat.m[1][2]*mat.m[2][0]*mat.m[3][1];
	return det;
}
	
inline float Det(const matrix3x3& mat)
{
	const float det =  
	mat.m[0][0] * mat.m[1][1] * mat.m[2][2] -
	mat.m[0][0] * mat.m[2][1] * mat.m[0][2] +
	mat.m[1][0] * mat.m[2][1] * mat.m[2][2] -
	mat.m[1][0] * mat.m[0][0] * mat.m[2][2] -
	mat.m[2][0] * mat.m[0][1] * mat.m[1][2] +
	mat.m[2][0] * mat.m[1][1] * mat.m[0][2];	
	return det;
}
	
inline float Det(const matrix2x2& mat)
{
	const float det = mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0];
	return det;
}

inline matrix2x2 Inverse(const matrix2x2& mat)
{
	const float det =  Det(mat);
	if (det == 0)
		return Identity();
	
	return matrix2x2(
		 mat.m[1][1] / det,
		-mat.m[0][1] / det,
		-mat.m[1][0] / det,
		 mat.m[0][0] / det
	);
}

inline matrix3x3 Inverse(const matrix3x3& mat)
{
	const float det =  Det(mat);
	if (det == 0)
		return Identity();
	
	return matrix3x3(
		 (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) / det,
		-(mat.m[0][1] * mat.m[2][2] - mat.m[0][2] * mat.m[2][1]) / det,
		-(mat.m[0][1] * mat.m[2][2] - mat.m[0][2] * mat.m[1][1]) / det,
		-(mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) / det,
		 (mat.m[0][0] * mat.m[2][2] - mat.m[0][2] * mat.m[2][0]) / det,
		-(mat.m[0][0] * mat.m[1][2] - mat.m[0][2] * mat.m[1][0]) / det,
		 (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]) / det,
		-(mat.m[0][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]) / det,
		 (mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]) / det
	);
}

inline matrix4x4 Inverse(const matrix4x4& mat)
{
	const float det = Det(mat);
	if (det == 0)
		return Identity();
	
	matrix4x4 iMat(
	 (  mat.m[1][1]*mat.m[2][2]*mat.m[3][3]
		+ mat.m[1][2]*mat.m[2][3]*mat.m[3][1]
		+ mat.m[1][3]*mat.m[2][1]*mat.m[3][2]
		- mat.m[1][1]*mat.m[2][3]*mat.m[3][2]
		- mat.m[1][2]*mat.m[2][1]*mat.m[3][3]
		- mat.m[1][3]*mat.m[2][2]*mat.m[3][1])/det,
	 (  mat.m[0][1]*mat.m[2][3]*mat.m[3][2]
		+ mat.m[0][2]*mat.m[2][1]*mat.m[3][3]
		+ mat.m[0][3]*mat.m[2][2]*mat.m[3][1]
		- mat.m[0][1]*mat.m[2][2]*mat.m[3][3]
		- mat.m[0][2]*mat.m[2][3]*mat.m[3][1]
		- mat.m[0][3]*mat.m[2][1]*mat.m[3][2])/det,
	 (  mat.m[0][1]*mat.m[1][2]*mat.m[3][3]
		+ mat.m[0][2]*mat.m[1][3]*mat.m[3][1]
		+ mat.m[0][3]*mat.m[1][1]*mat.m[3][2]
		- mat.m[0][1]*mat.m[1][3]*mat.m[3][2]
		- mat.m[0][2]*mat.m[1][1]*mat.m[3][3]
		- mat.m[0][3]*mat.m[1][2]*mat.m[3][1])/det,
	 (  mat.m[0][1]*mat.m[1][3]*mat.m[2][2]
		+ mat.m[0][2]*mat.m[1][1]*mat.m[2][3]
		+ mat.m[0][3]*mat.m[1][2]*mat.m[2][1]
		- mat.m[0][1]*mat.m[1][2]*mat.m[2][3]
		- mat.m[0][2]*mat.m[1][3]*mat.m[2][1]
		- mat.m[0][3]*mat.m[1][1]*mat.m[2][2])/det,
	 (  mat.m[1][0]*mat.m[2][3]*mat.m[3][2]
		+ mat.m[1][2]*mat.m[2][0]*mat.m[3][3]
		+ mat.m[1][3]*mat.m[2][2]*mat.m[3][0]
		- mat.m[1][0]*mat.m[2][2]*mat.m[3][3]
		- mat.m[1][2]*mat.m[2][3]*mat.m[3][0]
		- mat.m[1][3]*mat.m[2][0]*mat.m[3][2])/det,
	 (  mat.m[0][0]*mat.m[2][2]*mat.m[3][3]
		+ mat.m[0][2]*mat.m[2][3]*mat.m[3][0]
		+ mat.m[0][3]*mat.m[2][0]*mat.m[3][2]
		- mat.m[0][0]*mat.m[2][3]*mat.m[3][2]
		- mat.m[0][2]*mat.m[2][0]*mat.m[3][3]
		- mat.m[0][3]*mat.m[2][2]*mat.m[3][0])/det,
	 (  mat.m[0][0]*mat.m[1][3]*mat.m[3][2]
		+ mat.m[0][2]*mat.m[1][0]*mat.m[3][3]
		+ mat.m[0][3]*mat.m[1][2]*mat.m[3][0]
		- mat.m[0][0]*mat.m[1][2]*mat.m[3][3]
		- mat.m[0][2]*mat.m[1][3]*mat.m[3][0]
		- mat.m[0][3]*mat.m[1][0]*mat.m[3][2])/det,
	 (  mat.m[0][0]*mat.m[1][2]*mat.m[2][3]
		+ mat.m[0][2]*mat.m[1][3]*mat.m[2][0]
		+ mat.m[0][3]*mat.m[1][0]*mat.m[2][2]
		- mat.m[0][0]*mat.m[1][3]*mat.m[2][2]
		- mat.m[0][2]*mat.m[1][0]*mat.m[2][3]
		- mat.m[0][3]*mat.m[1][2]*mat.m[2][0])/det,
	 (  mat.m[1][0]*mat.m[2][1]*mat.m[3][3]
		+ mat.m[1][1]*mat.m[2][3]*mat.m[3][0]
		+ mat.m[1][3]*mat.m[2][0]*mat.m[3][1]
		- mat.m[1][0]*mat.m[2][3]*mat.m[3][1]
		- mat.m[1][1]*mat.m[2][0]*mat.m[3][3]
		- mat.m[1][3]*mat.m[2][1]*mat.m[3][0])/det,
	(  mat.m[0][0]*mat.m[2][3]*mat.m[3][1]
		+ mat.m[0][1]*mat.m[2][0]*mat.m[3][3]
		+ mat.m[0][3]*mat.m[2][1]*mat.m[3][0]
		- mat.m[0][0]*mat.m[2][1]*mat.m[3][3]
		- mat.m[0][1]*mat.m[2][3]*mat.m[3][0]
		- mat.m[0][3]*mat.m[2][0]*mat.m[3][1])/det,
	(  mat.m[0][0]*mat.m[1][1]*mat.m[3][3]
		+ mat.m[0][1]*mat.m[1][3]*mat.m[3][0]
		+ mat.m[0][3]*mat.m[1][0]*mat.m[3][1]
		- mat.m[0][0]*mat.m[1][3]*mat.m[3][1]
		- mat.m[0][1]*mat.m[1][0]*mat.m[3][3]
		- mat.m[0][3]*mat.m[1][1]*mat.m[3][0])/det,
	(  mat.m[0][0]*mat.m[1][3]*mat.m[2][1]
		+ mat.m[0][1]*mat.m[1][0]*mat.m[2][3]
		+ mat.m[0][3]*mat.m[1][1]*mat.m[2][0]
		- mat.m[0][0]*mat.m[1][1]*mat.m[2][3]
		- mat.m[0][1]*mat.m[1][3]*mat.m[2][0]
		- mat.m[0][3]*mat.m[1][0]*mat.m[2][1])/det,
	(  mat.m[1][0]*mat.m[2][2]*mat.m[3][1]
		+ mat.m[1][1]*mat.m[2][0]*mat.m[3][2]
		+ mat.m[1][2]*mat.m[2][1]*mat.m[3][0]
		- mat.m[1][0]*mat.m[2][1]*mat.m[3][2]
		- mat.m[1][1]*mat.m[2][2]*mat.m[3][0]
		- mat.m[1][2]*mat.m[2][0]*mat.m[3][1])/det,
	(  mat.m[0][0]*mat.m[2][1]*mat.m[3][2]
		+ mat.m[0][1]*mat.m[2][2]*mat.m[3][0]
		+ mat.m[0][2]*mat.m[2][0]*mat.m[3][1]
		- mat.m[0][0]*mat.m[2][2]*mat.m[3][1]
		- mat.m[0][1]*mat.m[2][0]*mat.m[3][2]
		- mat.m[0][2]*mat.m[2][1]*mat.m[3][0])/det,
	(  mat.m[0][0]*mat.m[1][2]*mat.m[3][1]
		+ mat.m[0][1]*mat.m[1][0]*mat.m[3][2]
		+ mat.m[0][2]*mat.m[1][1]*mat.m[3][0]
		- mat.m[0][0]*mat.m[1][1]*mat.m[3][2]
		- mat.m[0][1]*mat.m[1][2]*mat.m[3][0]
		- mat.m[0][2]*mat.m[1][0]*mat.m[3][1])/det,
	(  mat.m[0][0]*mat.m[1][1]*mat.m[2][2]
		+ mat.m[0][1]*mat.m[1][2]*mat.m[2][0]
		+ mat.m[0][2]*mat.m[1][0]*mat.m[2][1]
		- mat.m[0][0]*mat.m[1][2]*mat.m[2][1]
		- mat.m[0][1]*mat.m[1][0]*mat.m[2][2]
		- mat.m[0][2]*mat.m[1][1]*mat.m[2][0])/det);
	
	return iMat;
	
}


inline bool UnProject(const vec3& winpos,
					const matrix& invMatrix,
					const int viewport[4],
					vec3* ray)
{
	if (!ray)
		return false;
		
	// convert window coordinate
	vec4 inpos(winpos, 1.0);
	inpos.x = (inpos.x - viewport[0]) / viewport[2];
	inpos.y = (inpos.y - viewport[1]) / viewport[3];
	inpos = inpos * 2.0 - 1.0;
	inpos.w = 1.0;
	
	vec4 outpos = invMatrix * inpos;
	if (outpos.w == 0.0) return false;
	*ray = outpos.xyz() / outpos.w;
	return true;
}

inline bool UnProject(const vec3& winpos,
					const matrix& modelMatrix, 
					const matrix& projMatrix,
					const int viewport[4],
					vec3* ray)
{
	if (!ray)
		return false;

	matrix finalMatrix = projMatrix * modelMatrix;
	finalMatrix = Inverse(finalMatrix);
		
	return UnProject(winpos, finalMatrix, viewport, ray);
}


	
inline bool operator==(const matrix4x4& a, const matrix4x4& b)
{
	for (int i = 0; i < 16; i++)
	{
		if (a.f[i] != b.f[i])
			return false;
	}
	return true;
}
	
inline bool operator!=(const matrix4x4& a, const matrix4x4& b)
{
	for (int i = 0; i < 16; i++)
	{
		if (a.f[i] != b.f[i])
			return true;
	}
	return false;
}

inline bool operator==(const matrix3x3& a, const matrix3x3& b)
{
	for (int i = 0; i < 9; i++)
	{
		if (a.f[i] != b.f[i])
			return false;
	}
	return true;
}

inline bool operator!=(const matrix3x3& a, const matrix3x3& b)
{
	for (int i = 0; i < 9; i++)
	{
		if (a.f[i] != b.f[i])
			return true;
	}
	return false;
}

inline bool operator==(const matrix2x2& a, const matrix2x2& b)
{
	for (int i = 0; i < 4; i++)
	{
		if (a.f[i] != b.f[i])
			return false;
	}
	return true;
}

inline bool operator!=(const matrix2x2& a, const matrix2x2& b)
{
	for (int i = 0; i < 4; i++)
	{
		if (a.f[i] != b.f[i])
			return true;
	}
	return false;
}
	
// ----------------------- quaternion --------------------------------

inline quaternion QuatIdentity()
{
	return quaternion(0,0,0,1);
}

inline quaternion operator+(const quaternion& a, const quaternion& b)
{
	return quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline quaternion operator-(const quaternion& a, const quaternion& b)
{
	return quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline quaternion operator*(const quaternion& a, const quaternion& b)
{
	return quaternion(a.y * b.z - a.z * b.y + a.w * b.x + a.x * b.w,
					  a.z * b.x - a.x * b.z + a.w * b.y + a.y * b.w,
					  a.x * b.y - a.y * b.x + a.w * b.z + a.z * b.w,
					  a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}
	
inline quaternion operator+(const quaternion& a, const float r)
{
	return quaternion(a.x + r, a.y + r, a.z + r, a.w + r);
}

inline quaternion operator+(const float r, const quaternion& a)
{
	return quaternion(a.x + r, a.y + r, a.z + r, a.w + r);
}

inline quaternion operator-(const quaternion& a, const float r)
{
	return quaternion(a.x - r, a.y - r, a.z - r, a.w - r);
}

inline quaternion operator-(const float r, const quaternion& a)
{
	return quaternion(r - a.x, r - a.y, r - a.z, r - a.w);
}

inline quaternion operator*(const quaternion& a, const float r)
{
	return quaternion(a.x * r, a.y * r, a.z * r, a.w * r);
}

inline quaternion operator*(const float r, const quaternion& a)
{
	return quaternion(a.x * r, a.y * r, a.z * r, a.w * r);
}

inline quaternion operator/(const quaternion& a, const float r)
{
	const float inv = 1.0f / r;
	return quaternion(a.x * inv, a.y * inv, a.z * inv, a.w * r);
}

inline quaternion operator/(const float r, const quaternion& a)
{
	return quaternion(r / a.x, r / a.y, r / a.z, r / a.w);
}

inline float dot(const quaternion& a, const quaternion& b) 
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline quaternion lerp(const quaternion& a, const quaternion& b, const float r)
{
	return (b - a) * r + a;
}
	
inline quaternion QuatSlerp(const quaternion& a, const quaternion& b, const float r)
{
	quaternion q3;
	const float qr = dot(a, b);
	const float ss = 1.0f - qr * qr;
	float sp;
	if (ss <= 0.0f || (sp = Sqrt(ss)) == 0.0f) {
		q3 = a;
	} else {
		const float ph = Acos(qr);
		const float pt = ph * r;
		const float t1 = Sin(pt) / sp;
		const float t0 = Sin(ph - pt) / sp;
		q3 = a * t0 + b * t1;
	}
	return q3;
}
	
inline quaternion QuatSlerpNoInvert(const quaternion& a, const quaternion& b, const float r)
{
	float d = dot(a, b);
	if (d > -0.95f && d < 0.95f)
	{
		float angle = Cos(d);
		return (a * Sin(angle*(1.0f - r)) + b * Sin(angle * r)) / Sin(angle);
	}
	else  // if the angle is small, use linear interpolation								
	{
		return lerp(a,b,r);	
	}
}

	
inline float QuatLength(const quaternion& a)
{
	return Sqrt(a.w * a.w + a.x * a.x + a.y * a.y + a.z * a.z);
}

inline quaternion QuatNormalize(const quaternion& a)
{
	return a / QuatLength(a);
}

	
inline bool operator==(const quaternion& a, const quaternion& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline bool operator!=(const quaternion& a, const quaternion& b)
{
	return (a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w);
}

inline quaternion& operator*=(quaternion& a, const quaternion& b)
{
	a = a * b; return a;
}

inline vec4 operator*(const quaternion& q, const vec4& v)
{
	const float w = -q.x * v.x - q.y * v.y - q.z * v.z;
	const float x =  q.y * v.z - q.z * v.y + q.w * v.x;
	const float y =  q.z * v.x - q.x * v.z + q.w * v.y;
	const float z =  q.x * v.y - q.y * v.x + q.w * v.z;
	return vec4(
		y * -q.z + z * q.y - w * q.x + x * q.w,
		z * -q.x + x * q.z - w * q.y + y * q.w,
		x * -q.y + y * q.x - w * q.z + z * q.w,
		w *  q.w - x * q.x - y * q.y - z * q.z
	);
}

inline quaternion QuatRotationAxis(const vec3& v, float degree)
{	
	const float rad = ToRadian(degree);
	const float s = Sin(rad * 0.5f);
	return quaternion(
		v.x * s,
		v.y * s,
		v.z * s,
		Cos(rad * 0.5f)	);
}
	
inline float QuatAxisAngle(const quaternion& q, vec3& v)
{	
	float angle = Cos(q.w);
	const float sin_inv = 1.0f / Sin(angle);
	v.x = q.x * sin_inv;
	v.y = q.y * sin_inv;
	v.z = q.z * sin_inv;
	
	angle *= 2.0f;
	return angle;
}
	
inline quaternion QuatRotationYawPitchRoll(float yaw, float pitch, float roll)
{
	const float cos_z = Cos(roll  * 0.5f);
	const float cos_y = Cos(pitch * 0.5f);
	const float cos_x = Cos(yaw   * 0.5f);
	const float sin_z = Sin(roll  * 0.5f);
	const float sin_y = Sin(pitch * 0.5f);
	const float sin_x = Sin(yaw   * 0.5f);
	
	return quaternion(
		cos_z * cos_y * sin_x - sin_z * sin_y * cos_x,
		cos_z * sin_y * cos_x + sin_z * cos_y * sin_x,
		sin_z * cos_y * cos_x - cos_z * sin_y * sin_x,
		cos_z * cos_y * cos_x + sin_z * sin_y * sin_x);
}

inline quaternion ToQuaternion(const matrix3x3& m)
{
	const float tr = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f;
	if (tr >= 1.0f)
	{
		const float s = 0.5f / Sqrt(tr);
		return quaternion(
			(m.m[1][2] - m.m[2][1]) * s,
			(m.m[2][0] - m.m[0][2]) * s,
			(m.m[0][1] - m.m[1][0]) * s,
			0.25f / s);
	}
	else
	{
		float maxv;
		if(m.m[1][1] > m.m[2][2])    maxv = m.m[1][1];
		else                 maxv = m.m[2][2];
		
		if (maxv < m.m[0][0])
		{
			float s = Sqrt(m.m[0][0] - (m.m[1][1] + m.m[2][2]) + 1.0f);
			const float x = s * 0.5f;
			s = 0.5f / s;
			return quaternion(
				x,
				(m.m[0][1] + m.m[1][0]) * s,
				(m.m[2][0] + m.m[0][2]) * s,
				(m.m[1][2] - m.m[2][1]) * s);
		}
		else if (maxv == m.m[1][1])
		{
			float s = Sqrt(m.m[1][1] - (m.m[2][2] + m.m[0][0]) + 1.0f);
			const float y = s * 0.5f;
			s = 0.5f / s;
			return quaternion(
				(m.m[0][1] + m.m[1][0]) * s,
				y,
				(m.m[1][2] + m.m[2][1]) * s,
				(m.m[2][0] - m.m[0][2]) * s);
		}
		else
		{
			float s = Sqrt(m.m[2][2] - (m.m[0][0] + m.m[1][1]) + 1.0f);
			const float z = s * 0.5f;
			s = 0.5f / s;
			return quaternion(
				(m.m[2][0] + m.m[0][2]) * s,
				(m.m[1][2] + m.m[2][1]) * s,
				z,
				(m.m[0][1] - m.m[1][0]) * s);			
		}
	}
}

inline quaternion ToQuaternion(const matrix4x4& m)
{
	return ToQuaternion(matrix3x3(m));
}

inline matrix4x4 ToMatrix(const quaternion& q)
{
	const float sx = q.x * q.x;
	const float sy = q.y * q.y;
	const float sz = q.z * q.z;
	const float cx = q.y * q.z;
	const float cy = q.x * q.z;
	const float cz = q.x * q.y;
	const float wx = q.w * q.x;
	const float wy = q.w * q.y;
	const float wz = q.w * q.z;
	return matrix4x4(
		1.0f - 2.0f * (sy + sz),
		2.0f * (cz + wz),
		2.0f * (cy - wy),
		0.0f,
		2.0f * (cz - wz),
		1.0f - 2.0f * (sx + sz),
		2.0f * (cx + wx),
 		0.0f,
		2.0f * (cy + wy),
		2.0f * (cx - wx),
		1.0f - 2.0f * (sx + sy),
		0.0f,
		0.0f,0.0f,0.0f,1.0f
	);
}
	
	
inline vec3 HSVtoRGB(float H, float S, float V)
{
	vec3 rgb;
	int i;
	float fl;
	float m, n;
	i = (int)floor( H / 60 );
	fl = ( H / 60 ) - i;
	if( !(i & 1)) fl = 1 - fl; // if i is even
	
	m = V * ( 1 - S );
	n = V * ( 1 - S * fl );

#if 1
	const float table[] = {m,m,n,V,V,n,m,m,n,V};
	rgb.x = table[i + 4];
	rgb.y = table[i + 2];
	rgb.z = table[i    ];
#else
	switch( i ){
		case 0: rgb.x = V; rgb.y = n; rgb.z = m; break;
		case 1: rgb.x = n; rgb.y = V; rgb.z = m; break;
		case 2: rgb.x = m; rgb.y = V; rgb.z = n; break;
		case 3: rgb.x = m; rgb.y = n; rgb.z = V; break;
		case 4: rgb.x = n; rgb.y = m; rgb.z = V; break;
		case 5: rgb.x = V; rgb.y = m; rgb.z = n; break;
	}
#endif
	return rgb;
}
	
	
	// Intersect Triagle
inline bool IntersectTriangle(const vec3& org, const vec3& dir,
						    const vec3& v0, const vec3 v1, const vec3& v2,
							float& t, float& u, float& v)
{
	static const float EPSILON = 1.0e-24f;//0.000001f;
	
	const vec3 edge1 = v1 - v0;
	const vec3 edge2 = v2 - v0;
	const vec3 pvec = cross(dir, edge2);
	
	const float det = dot(edge1, pvec);
	// float side hit test	
	if (det > -EPSILON && det < EPSILON)
		return false;
	
	const float inv_det = 1.0f / det;
	const vec3 tvec = org - v0;
	u = dot(tvec, pvec) * inv_det;
	if (u < 0.0f || u > 1.0f)
		return false;
	
	const vec3 qvec = cross(tvec, edge1);
	v = dot(dir, qvec) * inv_det;
	if (v < -0.0f || u + v > 1.0f)
		return false;
	
	t = dot(edge2, qvec) * inv_det;
	return true;
}

inline float saturate(float x)
{
	return ((x < 0.0f ? 0.0f : x) > 1.0f ? 1.0f : x);
}
	
inline float smoothstep (float a, float b, float x)
{
	x = saturate((x - a) / (b - a));
	return x*x*(3-2*x);
}

// ===================  MultiComponent Index =======================
struct idx2
{
	int x,y;

	idx2()                                       { x = 0; y = 0;        }
	idx2(int x_, int y_)                         { x = x_; y = y_;      }
	idx2(const int* const v)                     { x = v[0]; y = v[1];  }
	idx2 operator-() const                       { return idx2(-x, -y); }
	int & operator[](int i)                      { return (&x)[i];      }
	int const & operator[](int i) const          { return (&y)[i];      }
	idx2& operator+=(const idx2& a) { x += a.x; y += a.y; return *this; }
	idx2& operator-=(const idx2& a) { x -= a.x; y -= a.y; return *this; }
	idx2& operator+=(int s)         { x += s;   y += s;   return *this; }
	idx2& operator-=(int s)         { x -= s;   y -= s;   return *this; }
	idx2& operator*=(int s)         { x *= s;   y *= s;   return *this; }
};

struct idx3
{
	int x,y,z;

	idx3()                                        { x = 0; y = 0; z = 0;          }
	idx3(const idx2& v, int z_ = 0)               { x = v.x; y = v.y; z = z_;     }
	idx3(int x_, int y_, int z_)                  { x = x_; y = y_; z = z_;       }
	idx3(const int* const v)                      { x = v[0]; y = v[1]; z = v[2]; }
	idx3 operator-() const                        { return idx3(-x, -y, -z);      }
	int & operator[](int i)                       { return (&x)[i];               }
    int const & operator[](int i) const           { return (&x)[i];               }
	idx3& operator+=(const idx3& a) { x += a.x; y += a.y; z += a.z; return *this; }
    idx3& operator-=(const idx3& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
    idx3& operator+=(int s)         { x += s;   y += s;   z += s;   return *this; }
    idx3& operator-=(int s)         { x -= s;   y -= s;   z -= s;   return *this; }
	idx3& operator*=(int s)         { x *= s;   y *= s;   z *= s;   return *this; }
};

// ----------------------------- idx3 -----------------------------------

inline idx3 operator+(const idx3& a, const idx3& b)
{
	return idx3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline idx3 operator+(const int r, const idx3& a)
{
	return idx3(a.x + r, a.y + r, a.z + r);
}

inline idx3 operator+(const idx3& a, const int r)
{
	return idx3(a.x + r, a.y + r, a.z + r);
}

inline idx3 operator-(const idx3& a, const idx3& b)
{
	return idx3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline idx3 operator-(const int r, const idx3& a)
{
	return idx3(a.x - r, a.y - r, a.z - r);
}

inline idx3 operator-(const idx3& a, const int r)
{
	return idx3(a.x - r, a.y - r, a.z - r);
}

inline idx3 operator*(const int r, const idx3& a)
{
	return idx3(a.x * r, a.y * r, a.z * r);
}

inline idx3 operator*(const idx3& a, const int r)
{
	return idx3(a.x * r, a.y * r, a.z * r);
}

inline bool operator==(const idx3& a, const idx3& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool operator!=(const idx3& a, const idx3& b)
{
	return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
}

// ----------------------------- idx2 -----------------------------------

inline idx2 operator+(const idx2& a, const idx2& b)
{
	return idx2(a.x + b.x, a.y + b.y);
}

inline idx2 operator+(const int r, const idx2& a)
{
	return idx2(a.x + r, a.y + r);
}

inline idx2 operator+(const idx2& a, const int r)
{
	return idx2(a.x + r, a.y + r);
}

inline idx2 operator-(const idx2& a, const idx2& b)
{
	return idx2(a.x - b.x, a.y - b.y);
}

inline idx2 operator-(const int r, const idx2& a)
{
	return idx2(a.x - r, a.y - r);
}

inline idx2 operator-(const idx2& a, const int r)
{
	return idx2(a.x - r, a.y - r);
}

inline idx2 operator*(const int r, const idx2& a)
{
	return idx2(a.x * r, a.y * r);
}

inline idx2 operator*(const idx2& a, const int r)
{
	return idx2(a.x * r, a.y * r);
}

inline bool operator==(const idx2& a, const idx2& b)
{
	return (a.x == b.x) && (a.y == b.y);
}

inline bool operator!=(const idx2& a, const idx2& b)
{
	return (a.x != b.x) || (a.y != b.y);
}
	
} // Math
} // VX

#endif // INCLUDE_VX_MATH_H
