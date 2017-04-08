/**
 * @file SimpleSTLB.h
 * STL形式ファイル簡易ローダー[バイナリデータ版]
 */
#ifndef INCLUDE_SIMPLESTLB
#define INCLUDE_SIMPLESTLB

/*
    SimpleSTLB
*/

#include "SimpleSTL.h"

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

inline unsigned int read4(const unsigned int* val, bool swapEndian)
{
    if (swapEndian) {
        return swap4(val);
    } else {
        return (*val);
    }
}

inline float read4(float* val, bool swapEndian)
{
    if (swapEndian) {
        return swap4(val);
    } else {
        return (*val);
    }
}


/**
 * SimpleSTLB
 */
class SimpleSTLB : public SimpleSTL
{
public:
    SimpleSTLB() : SimpleSTL() {}
	~SimpleSTLB(){}
	
  	bool Load(const char* filename, bool swapEndian = false, bool generateIndex = false)
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
		
		const unsigned int triCount = read4(reinterpret_cast<const unsigned int*>(&data[0]), swapEndian); data+=sizeof(unsigned int);
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
			m_pos[i+i0].x      = read4(&tri.vtx0[0], swapEndian);
			m_pos[i+i0].y      = read4(&tri.vtx0[1], swapEndian);
			m_pos[i+i0].z      = read4(&tri.vtx0[2], swapEndian);
			m_normal[i+i0].x   = read4(&tri.normal[0], swapEndian);
			m_normal[i+i0].y   = read4(&tri.normal[1], swapEndian);
			m_normal[i+i0].z   = read4(&tri.normal[2], swapEndian);
			m_pos[i+i1].x    = read4(&tri.vtx1[0], swapEndian);
			m_pos[i+i1].y    = read4(&tri.vtx1[1], swapEndian);
			m_pos[i+i1].z    = read4(&tri.vtx1[2], swapEndian);
			m_normal[i+i1].x = read4(&tri.normal[0], swapEndian);
			m_normal[i+i1].y = read4(&tri.normal[1], swapEndian);
			m_normal[i+i1].z = read4(&tri.normal[2], swapEndian);
			m_pos[i+i2].x    = read4(&tri.vtx2[0], swapEndian);
			m_pos[i+i2].y    = read4(&tri.vtx2[1], swapEndian);
			m_pos[i+i2].z    = read4(&tri.vtx2[2], swapEndian);
			m_normal[i+i2].x = read4(&tri.normal[0], swapEndian);
			m_normal[i+i2].y = read4(&tri.normal[1], swapEndian);
			m_normal[i+i2].z = read4(&tri.normal[2], swapEndian);
						
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
	
};

#endif // INCLUDE_SIMPLESTLB

