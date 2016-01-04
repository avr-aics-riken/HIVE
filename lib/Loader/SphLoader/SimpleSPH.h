/**
 * @file SimpleSPH.h
 * SPH形式ファイル簡易ローダー
 */

//
//  SimpleSPH
//

#ifndef __SimpleSPH_HEADER__
#define __SimpleSPH_HEADER__

#include <stdio.h>

/**
 * SimpleSPH
 */
class SimpleSPH
{
	
private:
	
	size_t freadEx(void* buf, size_t a, size_t b, FILE* fp, bool swap)
	{
		// 4byte only
		assert(a==4);
		assert(b==1);
		unsigned int ibuf;
		size_t s = fread(&ibuf, a, b, fp);
		if (swap)
			ibuf = ((ibuf & 0x000000FF)<<24)|
				   ((ibuf & 0x0000FF00)<<8 )|
				   ((ibuf & 0x00FF0000)>>8 )|
				   ((ibuf & 0xFF000000)>>24);
		*reinterpret_cast<unsigned int*>(buf) = ibuf;
		return s;
	}
	const float* sph_load(const char*& fname, int dim[3])
	{
		float* grid = 0;
		int len;
		int svType;
		int dType;
		int imax, jmax, kmax;
		float xorg, yorg, zorg;
		float xpitch, ypitch, zpitch;
		int step;
		float time;
		
		if (!fname)
			return 0;
		
		FILE* fp = fopen(fname, "rb");
		if (!fp)
			return 0;
		
		fread(&len, 4, 1, fp);
		bool enableswap = false;
		int eight = 8;
		if (len != eight) { // miss match endian
			enableswap = true;
			printf("enable ChangeEndian!!");
		}
		
		freadEx(&svType, 4, 1, fp,enableswap);
		freadEx(&dType, 4, 1, fp, enableswap);
		freadEx(&len, 4, 1, fp, enableswap);
		
		freadEx(&len, 4, 1,fp, enableswap);
		freadEx(&imax, 4, 1,fp, enableswap);
		freadEx(&jmax, 4, 1,fp, enableswap);
		freadEx(&kmax, 4, 1,fp, enableswap);
		freadEx(&len, 4, 1,fp, enableswap);
		
		freadEx(&len, 4, 1,fp, enableswap);
		freadEx(&xorg, 4, 1,fp, enableswap);
		freadEx(&yorg, 4, 1,fp, enableswap);
		freadEx(&zorg, 4, 1,fp, enableswap);
		freadEx(&len, 4, 1,fp, enableswap);
		
		freadEx(&len, 4, 1,fp, enableswap);
		freadEx(&xpitch, 4, 1,fp, enableswap);
		freadEx(&ypitch, 4, 1,fp, enableswap);
		freadEx(&zpitch, 4, 1,fp, enableswap);
		freadEx(&len, 4, 1,fp, enableswap);
		
		freadEx(&len, 4, 1,fp, enableswap);
		freadEx(&step, 4, 1,fp, enableswap);
		freadEx(&time, 4, 1,fp, enableswap);
		freadEx(&len, 4, 1,fp, enableswap);
		
		const char* endianstr[] = {"little","big"};
		printf("endian: %s\n", endianstr[(enableswap ? 1 : 0)]);
		printf("svType: %d\n", svType);
		printf("dType: %d\n", dType);
		printf("elements: %dx%dx%d\n", imax, jmax, kmax);
		printf("origin: %f %f %f\n", xorg, yorg, zorg);
		printf("pitch: %f %f %f\n", xpitch, ypitch, zpitch);
		printf("step: %d\n", step);
		printf("time: %f\n", time);
		
		dim[0] = imax;
		dim[1] = jmax;
		dim[2] = kmax;
		m_component = (svType == 2 ? 3 : 1);
		m_origin[0] = xorg;
		m_origin[1] = yorg;
		m_origin[2] = zorg;
		m_pitch[0] = xpitch;
		m_pitch[1] = ypitch;
		m_pitch[2] = zpitch;
		m_step = step;
		m_time = time;
		grid = new float[dim[0] * dim[1] * dim[2] * m_component];
		
		freadEx(&len, 4, 1, fp, enableswap);

		float* p = grid;
		for (int z=0; z<kmax; z++) {
			for (int y=0; y<jmax; y++) {
				for (int x=0; x<imax; x++) {
					for (int e=0; e<m_component; e++) {
						float data;
						freadEx(&data, 4, 1, fp, enableswap);
						//if (data != 0.0f)
						//	printf("%f ",data);
						*p = data;
						p++;
					}
				}
			}
		}
		freadEx(&len, 4, 1, fp, enableswap);
		
		fclose(fp);
		return grid;
	}
	
public:
	SimpleSPH()
	{
		m_dim[0] = 0;
		m_dim[1] = 0;
		m_dim[2] = 0;
		m_component = -1; // invalid
	}
	
	~SimpleSPH()
	{
	}
	
	const float* Load(const char* filename)
	{
		return sph_load(filename, m_dim);
	}
	
	int GetDim(int i)
	{
		if (i < 0 || i >= 3)
			return 0;
		return m_dim[i];
	}

	int GetComponent() {
		return m_component;
	}

  double GetTime() {
    return m_time;
  }

  double GetStep() {
    return m_step;
  }

  double GetOrigin(int i) {
    if (i < 0 || i >= 3) {
      return 0;
    }

    return m_origin[i];
  }

  double GetPitch(int i) {
    if (i < 0 || i >= 3) {
      return 0;
    }

    return m_pitch[i];
  }

private:
	int m_dim[3];
	int m_component;
	double m_origin[3];
	double m_pitch[3];
  double m_time;
  double m_step;
};

#endif /* defined(__SimpleSPH_HEADER__) */

