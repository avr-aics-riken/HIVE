/**
 * @file SimpleSTLB.h
 * STL形式ファイル簡易ローダー[バイナリデータ版]
 */
#ifndef INCLUDE_SIMPLESTLB
#define INCLUDE_SIMPLESTLB

/*
    SimpleSTLB
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#if 0
inline unsigned int swap4(const unsigned int* val)
{
	unsigned int tmp = *val;
	unsigned char *dst = (unsigned char*)&tmp;
	const unsigned char *src = (const unsigned char*)val;
	
	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];
	return tmp;
}

inline float swap4(float* f)
{
	unsigned int* d = reinterpret_cast<unsigned int*>(f);
	unsigned int i = swap4(d);
	return *reinterpret_cast<float*>(&i);
}
#else
inline float swap4(const unsigned int* i){ return *i; }
inline float swap4(float* f)       { return *f; }

#endif

/**
 * SimpleSTLB
 */
class SimpleSTLB
{
public:
	SimpleSTLB(){}
	~SimpleSTLB(){}
	
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
     * SimpleSTLB Triangle Info
     */
	struct triData
	{
		float normal[3];
		float vtx0[3];
		float vtx1[3];
		float vtx2[3];
		unsigned short pad;
	};
	bool Load(const char* filename, bool generateIndex = false)
	{
		FILE* fp = fopen(filename, "rb");
		if (!fp)
			return false;
		fseek(fp, 0, SEEK_END);
		size_t fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (fsize == 0){
			fclose(fp);
			return false;
		}
		unsigned char* fbuf = new unsigned char[fsize];
		fread(fbuf,fsize,1,fp);
		fclose(fp);
	
		const unsigned char* data = static_cast<const unsigned char*>(fbuf);
		data += 80; //skip header
		
		const unsigned int triCount = swap4(reinterpret_cast<const unsigned int*>(&data[0])); data+=sizeof(unsigned int);
		const unsigned int vertexCount = triCount * 3;
		
		m_pos.resize(vertexCount);
		m_normal.resize(vertexCount);
		if (generateIndex)
			m_face.resize(triCount);
		
		for (unsigned int i = 0; i < vertexCount; i+=3)
		{
			triData tri;
			memcpy(&tri, data, sizeof(tri));
			
			const unsigned int i0 = 0;
			const unsigned int i1 = 1;
			const unsigned int i2 = 2;
			m_pos[i+i0].x      = swap4(&tri.vtx0[0]);
			m_pos[i+i0].y      = swap4(&tri.vtx0[1]);
			m_pos[i+i0].z      = swap4(&tri.vtx0[2]);
			m_normal[i+i0].x   = swap4(&tri.normal[0]);
			m_normal[i+i0].y   = swap4(&tri.normal[1]);
			m_normal[i+i0].z   = swap4(&tri.normal[2]);
			m_pos[i+i1].x    = swap4(&tri.vtx1[0]);
			m_pos[i+i1].y    = swap4(&tri.vtx1[1]);
			m_pos[i+i1].z    = swap4(&tri.vtx1[2]);
			m_normal[i+i1].x = swap4(&tri.normal[0]);
			m_normal[i+i1].y = swap4(&tri.normal[1]);
			m_normal[i+i1].z = swap4(&tri.normal[2]);
			m_pos[i+i2].x    = swap4(&tri.vtx2[0]);
			m_pos[i+i2].y    = swap4(&tri.vtx2[1]);
			m_pos[i+i2].z    = swap4(&tri.vtx2[2]);
			m_normal[i+i2].x = swap4(&tri.normal[0]);
			m_normal[i+i2].y = swap4(&tri.normal[1]);
			m_normal[i+i2].z = swap4(&tri.normal[2]);
						
			if (tri.normal[0] == 0 && tri.normal[1] == 0 && tri.normal[2] == 0)
			{
				const vec3 v0 = m_pos[i  ];
				const vec3 v2 = m_pos[i+1];
				const vec3 v1 = m_pos[i+2];
				const vec3 N  = normalize(cross(v2 - v0, v1 - v0));
				m_normal[i]   = N;
				m_normal[i+1] = N;
				m_normal[i+2] = N;
			}
			
			data += 50;
		}
		if (generateIndex) {
			for (unsigned int i = 0; i < triCount; i++)
			{
				m_face[i].i0 = 3*i  ;
				m_face[i].i1 = 3*i+1;
				m_face[i].i2 = 3*i+2;
			}
		}
		
		delete [] fbuf;
		return true;
	}
	
	
	
private:
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

#endif // INCLUDE_SIMPLESTLB

