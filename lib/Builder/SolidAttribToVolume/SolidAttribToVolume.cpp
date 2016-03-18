/**
 * @file SolidAttribToVolume.cpp
 * Solid attrib データからVolumeDataへ変換するモジュール
 */
#include "SolidAttribToVolume.h"
#include "../Core/vxmath.h"
#include <string.h>
#include <algorithm>
#include <string>
#include <cfloat>

namespace {
    
    /**
     * BoundingBox[BB]中のpointデータのオフセットを求める
     * @param x X座標
     * @param y Y座標
     * @param z Z座標
     * @param bmin BB最小値
     * @param bmax ファイルパス
     * @param dim  ボリュームサイズ
     * @retval ボリュームデータ配列中のオフセット
     */
    inline size_t findLoc(float x, float y, float z, const float bmin[3], const float bmax[3], const size_t dim[3]) {
        float xx = (x - bmin[0]) / (bmax[0] - bmin[0]);
        float yy = (y - bmin[1]) / (bmax[1] - bmin[1]);
        float zz = (z - bmin[2]) / (bmax[2] - bmin[2]);
        
        size_t i = (std::max)((size_t)0, (std::min)((size_t)(dim[0]-1), (size_t)(xx*dim[0])));
        size_t j = (std::max)((size_t)0, (std::min)((size_t)(dim[1]-1), (size_t)(yy*dim[1])));
        size_t k = (std::max)((size_t)0, (std::min)((size_t)(dim[2]-1), (size_t)(zz*dim[2])));
        
        return k * dim[0] * dim[1] + j * dim[0] + i;
    }
    
    inline size_t Clamp(size_t x, size_t x0, size_t x1)
    {
        size_t i = (std::max)(x0, (std::min)(x1, x));
        
        return i;
    }
    
    struct Solid{
        typedef enum {
            SOLID_TETRA = 4,
            SOLID_PYRAMID = 5,
            SOLID_PRISM = 6,
            SOLID_HEXAHEDRON = 8,
        } SolidType;
        
        const SolidType type;
        
        std::vector<VX::Math::vec3> verts;
        std::vector<VX::Math::vec3> edges;
        std::vector<VX::Math::vec3> edge_pcs;
        
        Solid (float *verts_, int type_) : type((SolidType)type_){
            verts.resize(type);
            for(int i = 0; i < type; i++)
                verts[i] = VX::Math::vec3(verts_ + i*3);
            switch (type) {
                case SOLID_TETRA:
                    edges.resize(6);
                    edges[0] = verts[1]-verts[0];
                    edges[1] = verts[2]-verts[1];
                    edges[2] = verts[0]-verts[2];
                    edges[3] = verts[3]-verts[0];
                    edges[4] = verts[3]-verts[1];
                    edges[5] = verts[3]-verts[2];
                    edge_pcs.reserve(6);
                    for (int i = 0; i < 6; i++)
                        edge_pcs.push_back(cross(edges[i],verts[i%3]));
                    break;
                case SOLID_PYRAMID:
                    edges.resize(8);
                    edges[0] = verts[1]-verts[0];
                    edges[1] = verts[2]-verts[1];
                    edges[2] = verts[3]-verts[2];
                    edges[3] = verts[0]-verts[3];
                    edges[4] = verts[4]-verts[0];
                    edges[5] = verts[4]-verts[1];
                    edges[6] = verts[4]-verts[2];
                    edges[7] = verts[4]-verts[3];
                    edge_pcs.reserve(8);
                    for (int i = 0; i < 8; i++)
                        edge_pcs.push_back(cross(edges[i],verts[i%4]));
                    break;
                case SOLID_PRISM:
                    edges.resize(9);
                    edges[0] = verts[1]-verts[0];
                    edges[1] = verts[2]-verts[1];
                    edges[2] = verts[0]-verts[2];
                    edges[3] = verts[4]-verts[3];
                    edges[4] = verts[5]-verts[4];
                    edges[5] = verts[3]-verts[5];
                    edges[6] = verts[3]-verts[0];
                    edges[7] = verts[4]-verts[1];
                    edges[8] = verts[5]-verts[2];
                    edge_pcs.reserve(9);
                    for (int i = 0; i < 9; i++)
                        edge_pcs.push_back(cross(edges[i],verts[i%6]));
                    break;
                case SOLID_HEXAHEDRON:
                    edges.resize(12);
                    edges[0] = verts[1]-verts[0];
                    edges[1] = verts[2]-verts[1];
                    edges[2] = verts[3]-verts[2];
                    edges[3] = verts[0]-verts[3];
                    edges[4] = verts[5]-verts[4];
                    edges[5] = verts[6]-verts[5];
                    edges[6] = verts[7]-verts[6];
                    edges[7] = verts[4]-verts[7];
                    edges[8] = verts[4]-verts[0];
                    edges[9] = verts[5]-verts[1];
                    edges[10] = verts[6]-verts[2];
                    edges[11] = verts[7]-verts[3];
                    edge_pcs.reserve(12);
                    for (int i = 0; i < 12; i++)
                        edge_pcs.push_back(cross(edges[i],verts[i%8]));
                    break;
            }
        }
    };
    
    inline bool contain(const int &value, const int list[4]){
        if (list[0] == value) return true;
        if (list[1] == value) return true;
        if (list[2] == value) return true;
        if (list[3] == value) return true;
        return false;
    }
    
    inline int face_n(Solid::SolidType type){
        switch (type) {
            case Solid::SOLID_TETRA:
                return 4;
            case Solid::SOLID_PYRAMID:
                return 5;
            case Solid::SOLID_PRISM:
                return 5;
            case Solid::SOLID_HEXAHEDRON:
                return 6;
        }
        return 0;
    }
    
    // get cross point of Ray vs square face.
    inline VX::Math::vec3 getCP_sq(const VX::Math::vec3 &v0, const VX::Math::vec3 &v1,
                                   const VX::Math::vec3 &v2,
                                   const double &ws0, const double &ws1, const double &ws2,
                                   const double &ws3,
                                   const VX::Math::vec3 &edge2, const VX::Math::vec3 &edge3,
                                   const VX::Math::vec3 &raydir){
        double w = ws2 + ws3 + dot(cross(edge2, edge3),raydir);
        return (v2*ws0 + v0*ws1 + v1*w) / (ws0+ws1+w);
    }
    
    // kEPSの値は要検証. modeについて 0 : 0次連続, 1 ; 1次連続, 2 : n次連続
    float interpolate(const VX::Math::vec3 &p, const Solid &sld, const float *scalars, int mode = 1) {
        using namespace VX::Math;
        static const int faces[][6][4] = {
            {{}}, {{}}, {{}}, {{}},        // 0,1,2,3
            {{0,2,1,-1}, {1,2,3,-1}, {0,3,2,-1}, {0,1,3,-1} },  //tetra
            {{0,3,2,1}, {0,1,4,-1}, {1,2,4,-1}, {2,3,4,-1}, {0,4,3,-1} },  //pyramid
            {{0,2,1,-1}, {3,4,5,-1}, {0,3,5,2}, {0,1,4,3}, {1,2,5,4} },   //prism
            {{}},       // 7
            {{0,3,2,1}, {4,5,6,7}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {0,4,7,3}, },  //hexa
        };
        
        float weight[16]; // Alloc enough size. sld.type
        const float kEPS = 0.1;     //@fixme
        
        bool cw_f[2][16]; // Alloc enough size. sld.edges.size()
        float ws[16]; // Alloc enough size. sld.edges.size()
        
        for(int v = 0; v < sld.type; v++){
            
            vec3 rayorg(sld.verts[v]);
            vec3 raydir(normalize(p - sld.verts[v]));
            vec3 raypc(cross(raydir, rayorg));
            
            float d = length(p - rayorg);
            
            for(int i = 0; i < sld.edges.size(); i++){
                ws[i] = dot(raydir,sld.edge_pcs[i]) + dot(sld.edges[i],raypc);
                if(ws[i] >= -kEPS)  cw_f[0][i] = true;
                else cw_f[0][i] = false;
                if(ws[i] <= kEPS)  cw_f[1][i] = true;
                else cw_f[1][i] = false;
            }
            
            vec3 cp(1e16,1e16,1e16);
            
            switch(sld.type){
                case Solid::SOLID_TETRA:
                    if( cw_f[0][0] && cw_f[0][1] && cw_f[0][2] && !contain(v, faces[sld.type][0]) )
                        cp = (sld.verts[2]*ws[0] + sld.verts[0]*ws[1] + sld.verts[1]*ws[2])
                        / (ws[0]+ws[1]+ws[2]);
                    else if( cw_f[1][1] && cw_f[0][4] && cw_f[1][5] && !contain(v, faces[sld.type][1]) )
                        cp = (sld.verts[3]*ws[1] + sld.verts[1]*ws[5] - sld.verts[2]*ws[4])
                        / (ws[5]+ws[1]-ws[4]);
                    else if( cw_f[1][2] && cw_f[1][3] && cw_f[0][5] && !contain(v, faces[sld.type][2]) )
                        cp = (sld.verts[3]*ws[2] + sld.verts[2]*ws[3] - sld.verts[0]*ws[5])
                        / (ws[2]+ws[3]-ws[5]);
                    else if( cw_f[1][0] && cw_f[0][3] && cw_f[1][4] && !contain(v, faces[sld.type][3]) )
                        cp = (sld.verts[3]*ws[0] + sld.verts[0]*ws[4] - sld.verts[1]*ws[3])
                        / (ws[0]+ws[4]-ws[3]);
                    else {
                        weight[v] = 0;
                        continue;
                    }
                    break;
                case Solid::SOLID_PYRAMID:
                    if(cw_f[0][0] && cw_f[0][1] && cw_f[0][2] && cw_f[0][3] &&
                       !contain(v, faces[sld.type][0])){
                        cp = getCP_sq(sld.verts[0], sld.verts[1], sld.verts[2],
                                      ws[0], ws[1], ws[2], ws[3], sld.edges[2], sld.edges[3], raydir);
                    }else if (cw_f[1][0] && cw_f[1][5] && cw_f[0][4] &&
                              !contain(v, faces[sld.type][1])){
                        cp = (sld.verts[4]*-ws[0] + sld.verts[1]*ws[4] + sld.verts[0]*-ws[5])
                        / (-ws[0]+ws[4]-ws[5]);
                    }else if (cw_f[1][1] && cw_f[1][6] && cw_f[0][5] &&
                              !contain(v, faces[sld.type][2])){
                        cp = (sld.verts[4]*-ws[1] + sld.verts[2]*ws[5] + sld.verts[1]*-ws[6])
                        / (-ws[1]+ws[5]-ws[6]);
                    }else if (cw_f[1][2] && cw_f[1][7] && cw_f[0][6] &&
                              !contain(v, faces[sld.type][3])){
                        cp = (sld.verts[4]*-ws[2] + sld.verts[3]*ws[6] + sld.verts[2]*-ws[7])
                        / (-ws[2]+ws[6]-ws[7]);
                    }else if (cw_f[1][3] && cw_f[1][4] && cw_f[0][7] &&
                              !contain(v, faces[sld.type][4])){
                        cp = (sld.verts[4]*-ws[3] + sld.verts[0]*ws[7] + sld.verts[3]*-ws[4])
                        / (-ws[3]+ws[7]-ws[4]);
                    }else{
                        weight[v] = 0;
                        continue;
                    }
                    break;
                case Solid::SOLID_PRISM:
                    if(cw_f[0][0] && cw_f[0][1] && cw_f[0][2] &&
                       !contain(v, faces[sld.type][0])){
                        cp = (sld.verts[2]*ws[0] + sld.verts[0]*ws[1] + sld.verts[1]*ws[2])
                        / (ws[0]+ws[1]+ws[2]);
                    }else if(cw_f[1][3] && cw_f[1][4] && cw_f[1][5] &&
                             !contain(v, faces[sld.type][1])){
                        cp = (sld.verts[5]*ws[3] + sld.verts[3]*ws[4] + sld.verts[4]*ws[5]) / (ws[3]+ws[4]+ws[5]);
                    }else if(cw_f[1][2] && cw_f[1][6] && cw_f[0][5] && cw_f[0][8] &&
                             !contain(v, faces[sld.type][2])){
                        cp = getCP_sq(sld.verts[2], sld.verts[0], sld.verts[3],
                                      ws[2], ws[6], -ws[5], -ws[8], sld.edges[5], sld.edges[8], raydir);
                    }else if(cw_f[1][0] && cw_f[1][7] && cw_f[0][3] && cw_f[0][6] &&
                             !contain(v, faces[sld.type][3])){
                        cp = getCP_sq(sld.verts[0], sld.verts[1], sld.verts[4],
                                      ws[0], ws[7], -ws[3], -ws[6], sld.edges[3], sld.edges[6], raydir);
                    }else if(cw_f[1][1] && cw_f[1][8] && cw_f[0][4] && cw_f[0][7] &&
                             !contain(v, faces[sld.type][4])){
                        cp = getCP_sq(sld.verts[1], sld.verts[2], sld.verts[5],
                                      ws[1], ws[8], -ws[4], -ws[7], sld.edges[4], sld.edges[7], raydir);
                    }else{
                        weight[v] = 0;
                        continue;
                    }
                    break;
                case Solid::SOLID_HEXAHEDRON:
                    if     (cw_f[0][0] && cw_f[0][1] && cw_f[0][2] && cw_f[0][3] &&
                            !contain(v, faces[sld.type][0])){
                        cp = getCP_sq(sld.verts[0], sld.verts[1], sld.verts[2],
                                      ws[0], ws[1], ws[2], ws[3], sld.edges[2], sld.edges[3],raydir);
                    }else if(cw_f[1][4] && cw_f[1][5] && cw_f[1][6] && cw_f[1][7] &&
                             !contain(v, faces[sld.type][1])){
                        cp = getCP_sq(sld.verts[4], sld.verts[5], sld.verts[6],
                                      ws[4], ws[5], ws[6], ws[7], sld.edges[6], sld.edges[7],raydir);
                    }else if(cw_f[1][0] && cw_f[1][9] && cw_f[0][4] && cw_f[0][8] &&
                             !contain(v, faces[sld.type][2])){
                        cp = getCP_sq(sld.verts[0], sld.verts[1], sld.verts[5],
                                      ws[0], ws[9], -ws[4], -ws[8], sld.edges[4], sld.edges[8],raydir);
                    }else if(cw_f[1][1] && cw_f[1][10] && cw_f[0][5] && cw_f[0][9] &&
                             !contain(v, faces[sld.type][3])){
                        cp = getCP_sq(sld.verts[1], sld.verts[2], sld.verts[6],
                                      ws[1], ws[10], -ws[5], -ws[9], sld.edges[5], sld.edges[9],raydir);
                    }else if(cw_f[1][2] && cw_f[1][11] && cw_f[0][6] && cw_f[0][10] &&
                             !contain(v, faces[sld.type][4])){
                        cp = getCP_sq(sld.verts[2], sld.verts[3], sld.verts[7],
                                      ws[2], ws[11], -ws[6], -ws[10], sld.edges[6], sld.edges[10],raydir);
                    }else if(cw_f[1][3] && cw_f[1][8] && cw_f[0][7] && cw_f[0][11] &&
                             !contain(v, faces[sld.type][5])){
                        cp = getCP_sq(sld.verts[3], sld.verts[0], sld.verts[4],
                                      ws[3], ws[8], -ws[7], -ws[11], sld.edges[7], sld.edges[11],raydir);
                        
                    }else{
                        weight[v] = 0;
                        continue;
                    }
                    break;
            }
            
            d /= length(cp - rayorg);
            
            switch (mode) {
                case 0:
                    weight[v] = 1 - d;          //連続
                    break;
                case 1:
                    weight[v] = d*d*(d-1.5) + 0.5; //1次連続
                    break;
                case 2:
                    weight[v] = cos(PI*d)+1;    //n次微分が連続
                    break;
            }
        }
        
        float w = 0;
        float v = 0;
        for (int i = 0; i < sld.type; i ++){
            w += weight[i];
            v = v + scalars[i] * weight[i];
        }
        return v/w;
    }
    
    bool isPointInSolid (VX::Math::vec3 p, const Solid &solid){
        static const int faces[][6][4] = {
            {{}}, {{}}, {{}}, {{}},        // 0,1,2,3
            {{0,2,1,-1}, {1,2,3,-1}, {0,3,2,-1}, {0,1,3,-1} },  //tetra
            {{0,3,2,1}, {0,1,4,-1}, {1,2,4,-1}, {2,3,4,-1}, {0,4,3,-1} },  //pyramid
            {{0,2,1,-1}, {3,4,5,-1}, {0,3,5,2}, {0,1,4,3}, {1,2,5,4} },   //prism
            {{}},       // 7
            {{0,3,2,1}, {4,5,6,7}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {0,4,7,3}, },  //hexa
        };
        float d = -1;
        for(int i = 0; i < face_n(solid.type); i ++)
            d = std::max(d, dot(normalize(cross(solid.verts[faces[solid.type][i][1]]
                                                - solid.verts[faces[solid.type][i][0]],
                                                solid.verts[faces[solid.type][i][2]]
                                                - solid.verts[faces[solid.type][i][1]])),
                                p - solid.verts[faces[i][solid.type][0]] ) );
        
        return d < 0;
    }
    
}

/**
 * 多面体データからVolumeDataへの変換
 * @param w Width
 * @param h Height
 * @retval true 変換成功
 * @retval false 変換失敗
 */
bool SolidAttribToVolume::ToVolume(int w, int h, int d) {
    
    m_volume = BufferVolumeData::CreateInstance();
    
    int c = 1; // Store scalar density.
    m_volume->Create(w, h, d, c);
    
    //    assert(m_solid);
    //    assert(m_solid->Position());
    //    assert(m_solid->Position()->GetNum() > 0);
    
    float* voxels = m_volume->Buffer()->GetBuffer();
    
    const size_t fnum = w * h * d * c;
    for (size_t i = 0; i < fnum; i++) {
        voxels[i] = 0.0f; //-FLT_MAX; // @fixme
    }
    
    std::vector<int> countBuf(w * h * d, 0); // for compute avarage.
    
    float *position = m_solid->Position()->GetBuffer();
    float *attrib   = m_attrib->Float()->GetBuffer();
    
    size_t dim[3] = { w, h, d };
    
    for (int i = 0; i < m_solid->Index()->GetNum(); i++) {
        float *vertices_f =  position + 3 * m_solid->Type() * i;
        float bmax_[3], bmin_[3];
        
        bmin_[0] = bmax_[0] = vertices_f[0];
        bmin_[1] = bmax_[1] = vertices_f[1];
        bmin_[2] = bmax_[2] = vertices_f[2];
        
        for (int j = 1; j < m_solid->Type(); j++) {
            
            bmin_[0] = (std::min)(bmin_[0], vertices_f[3*j+0]);
            bmin_[1] = (std::min)(bmin_[1], vertices_f[3*j+1]);
            bmin_[2] = (std::min)(bmin_[2], vertices_f[3*j+2]);
            
            bmax_[0] = (std::max)(bmax_[0], vertices_f[3*j+0]);
            bmax_[1] = (std::max)(bmax_[1], vertices_f[3*j+1]);
            bmax_[2] = (std::max)(bmax_[2], vertices_f[3*j+2]);
        }
        
        Solid solid(vertices_f, m_solid->Type());
        
        for (int x = bmin_[0]; x <= bmax_[0]; x++)
          for (int y = bmin_[1]; y <= bmax_[1]; y++)
            for (int z = bmin_[2]; z <= bmax_[2]; z++){
              VX::Math::vec3 p(x, y, z);
              if(isPointInSolid(p, solid)){
                  size_t loc = findLoc(x, y, z, m_bmin, m_bmax, dim);
                  voxels[loc] += interpolate(p, solid,
                                             attrib + i * m_solid->Type());
                  countBuf[loc]++;
              }
            
            }
    }
    
    // Take an avarage.
    for (size_t i = 0; i < size_t(w) * size_t(h) * size_t(d); i++) {
        if (countBuf[i] > 0) {
            voxels[i] /= (float)countBuf[i];
        } else {
            //printf("empty: %d\n", i);
        }
    }
    
    printf("ToVolume: %zu, %zu, %zu\n", dim[0], dim[1], dim[2]);
    
    return true;
}

/**
 * Create volume from solid attribute.
 * @param solid 変換対象の solid データ
 * @param attrib 変換対象の Attrib データ
 * @retval 1 変換成功
 * @retval 0 変換失敗
 */
int SolidAttribToVolume::Create(BufferSolidData *solid, BufferExtraData* attrib, float bmin[3], float bmax[3]) {
    if (!solid) {
        return 0;
    }
    
    if (solid->Position()->GetNum() < 1) {
        return 0;
    }
    
    // @todo { Support vector type. }
    {
        std::string type = std::string(attrib->GetDataType());
        if (type != "float") {
            return 0;
        }
    }
    
    if (attrib->Float()->GetNum() < 1) {
        return 0;
    }
    
    m_solid = solid;
    m_attrib = attrib;
    
    m_bmin[0] = bmin[0];
    m_bmin[1] = bmin[1];
    m_bmin[2] = bmin[2];
    
    m_bmax[0] = bmax[0];
    m_bmax[1] = bmax[1];
    m_bmax[2] = bmax[2];
    
    printf("SolidAttribToVolume: bmin = %f, %f, %f\n", bmin[0], bmin[1], bmin[2]);
    printf("SolidAttribToVolume: bmax = %f, %f, %f\n", bmax[0], bmax[1], bmax[2]);
    
    return 1;
}

/**
 * VolumeDataへの参照取得1
 * @retval m_volume VolumeDataへの参照
 */
BufferVolumeData* SolidAttribToVolume::VolumeData()
{
    return m_volume;
}

/// コンストラクタ
SolidAttribToVolume::SolidAttribToVolume() : m_volume(0), m_solid(0) {
}

/// デストラクタ
SolidAttribToVolume::~SolidAttribToVolume() {
}

