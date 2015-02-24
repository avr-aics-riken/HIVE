#include "VolumeToVector.h"
#include "../Core/vxmath.h"
#include <string.h>


inline void GenerateTetra(
  float* out_vertices,
  float* in_vertices, double bmin[3], double bmax[3], double scale)
{
  double u0 = in_vertices[0];
  double u1 = in_vertices[1];
  double u2 = in_vertices[2];

  //TODO const
  double offset[4][3];
  offset[0][0] = 1.0;
  offset[0][1] = 0.0;
  offset[0][2] = 0.0;
  offset[1][0] = 1.0;
  offset[1][1] = 0.0;
  offset[1][2] = 1.0;
  offset[2][0] = 0.0;
  offset[2][1] = 0.0;
  offset[2][2] = 1.0;
  offset[3][0] = 0.0;
  offset[3][1] = 1.0;
  offset[3][2] = 0.0;

  double px = (bmax[0] - bmin[0]) * u0 + bmin[0];
  double py = (bmax[1] - bmin[1]) * u1 + bmin[1];
  double pz = (bmax[2] - bmin[2]) * u2 + bmin[2];
  for (size_t j = 0; j < 4; j++) {
    double v0 = scale * (bmax[0] - bmin[0]) * offset[j][0];
    double v1 = scale * (bmax[1] - bmin[1]) * offset[j][1];
    double v2 = scale * (bmax[2] - bmin[2]) * offset[j][2];
    out_vertices[3 * (4 * j) + 0] = px + v0;
    out_vertices[3 * (4 * j) + 1] = py + v1;
    out_vertices[3 * (4 * j) + 2] = pz + v2;
  }
}


VolumeToVector::VolumeToVector(){
    m_tetra = new BufferTetraData();
    m_line  = new BufferLineData();
    m_pitchX = 1.0;
    m_pitchY = 1.0;
    m_pitchZ = 1.0;
}


void VolumeToVector::DividePitchX(double a) {
    if(a <= 0) {
        printf("ERROR : Invalid PitchX\n");
        return ;
    }
    m_pitchX = a;
}

void VolumeToVector::DividePitchY(double a) {
    if(a <= 0) {
        printf("ERROR : Invalid PitchY\n");
        return ;
    }
    m_pitchY = a;
}

void VolumeToVector::DividePitchZ(double a) {
    if(a <= 0) {
        printf("ERROR : Invalid PitchZ\n");
        return ;
    }
    m_pitchZ = a;
}

void VolumeToVector::DivideNumberX(double a) {
    DividePitchX(a);
}

void VolumeToVector::DivideNumberY(double a) {
    DividePitchY(a);
}

void VolumeToVector::DivideNumberZ(double a) {
    DividePitchZ(a);
}


int VolumeToVector::Create(BufferVolumeData *volume, double scale, double radius_scale) {
    if (!volume || scale <= 0.0) {
        return 0;
    }

    //todo custom
    const double bmin[3]          = {-1, -1, -1};
    const double bmax[3]          = { 1,  1,  1};
    const int    width            = volume->Width();
    const int    height           = volume->Height();
    const int    depth            = volume->Depth();
    const int    volume_num       = (width * height * depth);
    const int    line_vertex_num  = volume_num * 2;
    const int    indexnum         = 0;//obj.GetIndexNum()
    const float  offset_inc       = 1.0;

    m_line->Create(line_vertex_num, indexnum);
    UintBuffer*  idx     = m_line->Index();
    FloatBuffer* mat     = m_line->Material();
    FloatBuffer* radius  = m_line->Radius();
    float*       rad     = radius->GetBuffer();
    if (rad) {
        for (int i = 0; i < line_vertex_num; ++i) {
            rad[i] = static_cast<float>(radius_scale);
        }
    }
    memset(mat->GetBuffer(), 0, sizeof(float) * mat->GetNum());

    //Create Line Data
    Vec3Buffer*     pos        = reinterpret_cast<Vec3Buffer*>(m_line->Position());
    Vec3Buffer*     volumedata = reinterpret_cast<Vec3Buffer*>(volume->Buffer());
    VX::Math::vec3* volbuf     = reinterpret_cast<VX::Math::vec3*>(volumedata->GetBuffer());
    VX::Math::vec3* linebuf    = reinterpret_cast<VX::Math::vec3*>(pos->GetBuffer());

    //Create Buffer
#if 0
    float offsetZ    = static_cast<float>(-depth  / 2) + 0.5;
    for(int k = 0; k < depth; k++) {
        float offsetY    = static_cast<float>(-height / 2) - 0.5; //yUP
        for(int j = 0; j < height; j++) {
            float offsetX    = static_cast<float>(-width  / 2) + 0.5;
            for(int i = 0; i < width; i++) {
                VX::Math::vec3 v0 = VX::Math::vec3(offsetX, offsetY, offsetZ);
                VX::Math::vec3 v1 = *volbuf;
                volbuf++;
                v1 = normalize(v1);

                *linebuf++ = v0;
                *linebuf++ = v0 + v1;
                
                offsetX += offset_inc;
            }
            offsetY += offset_inc;
        }
        offsetZ += offset_inc;
    }
#endif

    const double  fwidth      = static_cast<double>(width );
    const double  fheight     = static_cast<double>(height);
    const double  fdepth      = static_cast<double>(depth );
    const double  offset_incX = m_pitchX;
    const double  offset_incY = m_pitchY;
    const double  offset_incZ = m_pitchZ;
    float offsetZ    = static_cast<float>(-depth  / 2) + 0.5;
    for(double k = 0; k < fdepth; k += offset_incZ)
    {
        float offsetY    = static_cast<float>(-height / 2) - 0.5; //yUP
        for(double j = 0; j < fheight; j += offset_incY)
        {
            float offsetX    = static_cast<float>(-width / 2) + 0.5;
            for(double i = 0; i < fwidth; i += offset_incX)
            {
                int buf_offset  = static_cast<int>(i);                  //X
                buf_offset     += static_cast<int>(j) * width;          //Y
                buf_offset     += static_cast<int>(k) * width * height; //Z
                if(volumedata->GetNum() <= buf_offset) {
                    printf("ERROR : BOF bailout.\n");
                    goto bailout;
                }

                VX::Math::vec3 v0 = VX::Math::vec3(offsetX, offsetY, offsetZ);
                VX::Math::vec3 v1 = volbuf[buf_offset];
                v1 = normalize(v1);

                *linebuf++ = v0;
                *linebuf++ = v0 + v1;

                offsetX += offset_incX;
            }
            offsetY += offset_incY;
        }
        offsetZ += offset_incZ;
    }

    /*
    //1cell 1tetra
    int tetra_vertex_num = volume_num * 4;
    
    //Create TetraData
    m_tetra->Create(tetra_vertex_num, tetra_vertex_num);
    float *dest          = m_tetra->Position()->GetBuffer();
    
    //GenerateTetra
    for (int i = 0; i < volume_num; ++i){
        GenerateTetra(&dest[i * 4], &source[i * 3], bmin, bmax, scale);
    }

    return m_tetra->Position()->GetNum();
    */
bailout:
    return m_tetra->Position()->GetNum();
}

BufferLineData* VolumeToVector::LineData()
{
    return m_line;
}

BufferTetraData* VolumeToVector::TetraData()
{
    return m_tetra;
}
