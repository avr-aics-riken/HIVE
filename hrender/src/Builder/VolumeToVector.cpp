/**
 * @file VolumeToVector.cpp  
 * VolumeDataからVectorメッシュデータを作成するモジュール
 */
#include "VolumeToVector.h"
#include "../Core/vxmath.h"
#include <string.h>


/**
 * Tetraメッシュの作成
 * @param out_vertices 出力頂点
 * @param in_vertices  出力頂点
 * @param bmin         BoundingBoxMin
 * @param bmax         BoundingBoxMax
 * @param scale        Tetraのサイズ
 */
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


/// コンストラクタ
VolumeToVector::VolumeToVector(){
    m_vector      = new BufferVectorData();
    m_pitchX      = 1.0;
    m_pitchY      = 1.0;
    m_pitchZ      = 1.0;
    m_pitchIntX   = 1;
    m_pitchIntY   = 1;
    m_pitchIntZ   = 1;
    m_usePitchInt = false;
}


/**
 * VolumeData X軸分割サイズ
 * @param  a     分割サイズ
 * @retval true  成功
 * @retval false 失敗
 */
bool VolumeToVector::DividePitchX(double a) {
    if(a <= 0) {
        printf("ERROR : Invalid PitchX\n");
        return false;
    }
    m_pitchX = a;
    return true;
}

/**
 * VolumeData Y軸分割サイズ
 * @param  a     分割サイズ
 * @retval true  成功
 * @retval false 失敗
 */
bool VolumeToVector::DividePitchY(double a) {
    if(a <= 0) {
        printf("ERROR : Invalid PitchY\n");
        return false;
    }
    m_pitchY = a;
    return true;
}

/**
 * VolumeData Z軸分割サイズ
 * @param  a     分割サイズ
 * @retval true  成功
 * @retval false 失敗
 */
bool VolumeToVector::DividePitchZ(double a) {
    if(a <= 0) {
        printf("ERROR : Invalid PitchZ\n");
        return false;
    }
    m_pitchZ = a;
    return true;
}

/**
 * VolumeData X軸分割数指定
 * @param  a     分割数
 * @retval true  成功
 * @retval false 失敗
 */
bool VolumeToVector::DivideNumberX(int a) {
    if(a <= 0) {
        printf("ERROR : Invalid NumberX\n");
        return false;
    }
    m_pitchIntX = a;
    return true;
}

/**
 * VolumeData Y軸分割数指定
 * @param  a     分割数
 * @retval true  成功
 * @retval false 失敗
 */
bool VolumeToVector::DivideNumberY(int a) {
    if(a <= 0) {
        printf("ERROR : Invalid NumberY\n");
        return false;
    }
    m_pitchIntY = a;
    return true;
}

/**
 * VolumeData Z軸分割数指定
 * @param  a     分割数
 * @retval true  成功
 * @retval false 失敗
 */
bool VolumeToVector::DivideNumberZ(int a) {
    if(a <= 0) {
        printf("ERROR : Invalid NumberZ\n");
        return false;
    }
    m_pitchIntZ = a;
    return true;
}

/**
 * VolumeData XYZ分割サイズ指定
 * @param  x  X軸分割サイズ
 * @param  y  Y軸分割サイズ
 * @param  z  Z軸分割サイズ
 * @retval true  成功
 */
bool VolumeToVector::DividePitch(double x, double y, double z) {
    DividePitchX(x);
    DividePitchY(y);
    DividePitchZ(z);
    m_usePitchInt = false;
    return true;
}

/**
 * VolumeData XYZ分割数指定
 * @param  x  X軸分割数
 * @param  y  Y軸分割数
 * @param  z  Z軸分割数
 * @retval true  成功
 */
bool VolumeToVector::DivideNumber(int x, int y, int z) {
    DivideNumberX(x);
    DivideNumberY(y);
    DivideNumberZ(z);
    m_usePitchInt = true;
    return true;
}

/**
 * VolumeData XYZベクトルデータ生成
 * @param  volume  X軸分割数
 * @retval 0 Volumeが指定されていない
 * @retval >0 生成されたベクトルデータの頂点数
 */
int VolumeToVector::Create(BufferVolumeData *volume) {
    if (!volume) {
        return 0;
    }

    //todo custom
    const double  bmin[3]      = {-1, -1, -1};
    const double  bmax[3]      = { 1,  1,  1};
    const int     width        = volume->Width();
    const int     height       = volume->Height();
    const int     depth        = volume->Depth();
    const int     volume_num   = (width * height * depth);
    const int     indexnum     = 0;//obj.GetIndexNum()
    const float   offset_inc   = 1.0;
    const double  fwidth       = static_cast<double>(width );
    const double  fheight      = static_cast<double>(height);
    const double  fdepth       = static_cast<double>(depth );
    double        offset_incX  = m_pitchX;
    double        offset_incY  = m_pitchY;
    double        offset_incZ  = m_pitchZ;
    if(m_usePitchInt) {
        printf("Use PIch Int\n");
        offset_incX = fwidth  / static_cast<double>(m_pitchIntX);
        offset_incY = fheight / static_cast<double>(m_pitchIntY);
        offset_incZ = fdepth  / static_cast<double>(m_pitchIntZ);
    }
    printf("VolumeNum  = %d(%d %d %d)\n", volume_num, width, height, depth);
    printf("OFFSET_INC = %f %f %f\n", offset_incX, offset_incY, offset_incZ);

    //Create Line Data
    Vec3Buffer*     volumedata = reinterpret_cast<Vec3Buffer*>(volume->Buffer());
    VX::Math::vec3* volbuf     = reinterpret_cast<VX::Math::vec3*>(volumedata->GetBuffer());

    std::vector<VX::Math::vec3> vposbuf;  //need reserve?
    std::vector<VX::Math::vec3> vdirbuf;  //need reserve?

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
                    printf("ERROR : BOF .\n");
                }

                VX::Math::vec3 v0 = VX::Math::vec3(offsetX, offsetY, offsetZ);
                VX::Math::vec3 v1 = volbuf[buf_offset];

				// Skip zero value voxel.
				if (fabsf(v1[0]) < 1.0e-20f && fabsf(v1[1]) < 1.0e-20f && fabsf(v1[2]) < 1.0e-20f) {
					continue;
				}

                v1 = normalize(v1);


                vposbuf.push_back(v0);
                vdirbuf.push_back(v1);

                offsetX += offset_incX;
            }
            offsetY += offset_incY;
        }
        offsetZ += offset_incZ;
    }
    printf("Builder : Line Num = %zu\n", vposbuf.size());
    m_vector->Create(vposbuf.size());
    Vec3Buffer*     pos     = reinterpret_cast<Vec3Buffer*>(m_vector->Position());
    VX::Math::vec3* posbuf  = reinterpret_cast<VX::Math::vec3*>(pos->GetBuffer());
    Vec3Buffer*     dir     = reinterpret_cast<Vec3Buffer*>(m_vector->Direction());
    VX::Math::vec3* dirbuf  = reinterpret_cast<VX::Math::vec3*>(dir->GetBuffer());

    //setup vertex
    memcpy(posbuf, &vposbuf[0], sizeof(VX::Math::vec3) * vposbuf.size());
    memcpy(dirbuf, &vdirbuf[0], sizeof(VX::Math::vec3) * vdirbuf.size());

    return vposbuf.size();
}

/**
 * 変換されたベクトルデータへの参照を取得する
 * @return 変換されたベクトルデータへの参照が返却される。
 */
BufferVectorData* VolumeToVector::VectorData()
{
    return m_vector;
}

