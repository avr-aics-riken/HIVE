//
//  SimpleVOL
//

#ifndef __SimpleVOL_HEADER__
#define __SimpleVOL_HEADER__

#include <stdio.h>


class SimpleVOL
{

private:

    static void swap4(unsigned int *val) {
        unsigned int tmp = *val;
        unsigned char *dst = (unsigned char *)val;
        unsigned char *src = (unsigned char *)&tmp;

        dst[0] = src[3];
        dst[1] = src[2];
        dst[2] = src[1];
        dst[3] = src[0];
    }

    static void swap8(unsigned long long *val) {
        unsigned long long tmp = *val;
        unsigned char *dst = (unsigned char *)val;
        unsigned char *src = (unsigned char *)&tmp;

        dst[0] = src[7];
        dst[1] = src[6];
        dst[2] = src[5];
        dst[3] = src[4];
        dst[4] = src[3];
        dst[5] = src[2];
        dst[6] = src[1];
        dst[7] = src[0];
    }

    float* loadVOL(const char*& filename, int dim[3], int& component)
    {
        FILE* fp = fopen(filename,"rb");
        if (!fp)
            return 0;

        printf("Loading:[%s]\n",filename);

        fseek(fp, 0, SEEK_END);
        const size_t fsize = ftell(fp);
        if (fsize>>20)
            printf(" size: %ld[MB]\n",(fsize>>20));
        else
            printf(" size: %ld[KB]\n",(fsize>>10));

        fseek(fp, 0, SEEK_SET);
        float* voldata = new float[fsize/sizeof(float)];
        fread(voldata, fsize, 1, fp);
        fclose(fp);

        int header_size = sizeof(int) * 5;

        unsigned int type = *reinterpret_cast<unsigned int*>(&voldata[fsize/sizeof(float)-5]);
        unsigned int components = *reinterpret_cast<unsigned int*>(&voldata[fsize/sizeof(float)-4]);
        unsigned int w = *reinterpret_cast<unsigned int*>(&voldata[fsize/sizeof(float)-3]);
        unsigned int h = *reinterpret_cast<unsigned int*>(&voldata[fsize/sizeof(float)-2]);
        unsigned int d = *reinterpret_cast<unsigned int*>(&voldata[fsize/sizeof(float)-1]);

        // type:
        //  0 - int8
        //  1 - uint8
        //  2 - int16
        //  3 - uint16
        //  4 - int32
        //  5 - uint32
        //  6 - int64
        //  7 - uint64
        //  8 - float32
        //  9 - float64

        // Hack. If value is huge, assume endian is different.
        // @fixme { use more robust way. }
        bool flip = false;
        if ((w > 65536) || (h > 65536) || (d > 65536)) {
            flip = true;
        }

        if (flip) {
            printf(" vol: %d x %d x %d\n", w, h, d);
            swap4(&components);
            swap4(&type);
            swap4(&w);
            swap4(&h);
            swap4(&d);

            if (type == 8) { // float array
                for (size_t i = 0; i < fsize / sizeof(float) - header_size; i++) {
                    swap4((unsigned int*)&voldata[i]);
                }
            } else if (type == 9) { // double array
                for (size_t i = 0; i < fsize / sizeof(double) - header_size; i++) {
                    swap8((unsigned long long*)&voldata[i]);
                }
            }
        }

        // Currently supported type is ... uint8, float32 and float64
        assert((type == 1) || (type == 8) || (type == 9));

        printf(" vol: %d x %d x %d x %d\n", w, h, d, components);

        dim[0] = w;
        dim[1] = h;
        dim[2] = d;
        component = components;
        size_t datasize;
        if (type == 1) {
            //type = VOLUME_TYPE_U8;
            datasize = sizeof(unsigned char);
        } else if (type == 8) {
            //type = VOLUME_TYPE_F32;
            datasize = sizeof(float);
        } else if (type == 9) {
            //type = VOLUME_TYPE_F64;
            datasize = sizeof(double);
        }

        return voldata;
    }


public:
    SimpleVOL()
    {
        m_dim[0] = 0;
        m_dim[1] = 0;
        m_dim[2] = 0;
        m_component = -1; // invalid
    }

    ~SimpleVOL()
    {
    }

    const float* Load(const char* filename)
    {
        return loadVOL(filename, m_dim, m_component);
    }

    int GetDim(int i)
    {
        if (i >= 3)
            return 0;
        return m_dim[i];
    }
    int GetComponent() {
        return m_component;
    }
private:
    int m_dim[3];
    int m_component;
};

#endif /* defined(__SimpleVOL_HEADER__) */
