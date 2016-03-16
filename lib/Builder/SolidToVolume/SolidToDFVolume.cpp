/**
 * @file SolidToDFVolume.cpp
 * 点データからVolumeDataへ変換するモジュール
 */
#include "SolidToDFVolume.h"
//#include "../Core/vxmath.h"
#include "vxmath.h"
#include <string.h>
#include <algorithm>

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
        std::vector<VX::Math::vec3> normals;
        
        Solid( float *verts_, int type_) : type((SolidType)type_){
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
                    edges[7] = verts[5]-verts[1];
                    edges[8] = verts[4]-verts[2];
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
                    edges[9] = verts[7]-verts[1];
                    edges[10] = verts[6]-verts[2];
                    edges[11] = verts[5]-verts[3];
                    break;
            }
        }
    };
    
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
    
    //立体からの距離
    inline float disntace_pt_sld(const VX::Math::vec3 &pt, const Solid &sld, const int ns[6][4]){
        float d = -1e16;
        float d_b[6];
        
        //内部にある場合
        for(int i = 0, n = face_n(sld.type); i < n; i ++){
            d_b[i] = dot(sld.normals[i],
                         pt - sld.vertexes[ns[i][0]]);
            if(d < d_b[i]) d = d_b[i];
        }
        if(d < 0){
            return d;
        }
        
        //面に一番近い場合
        for(int i = 0; i < sld.normals.size(); i ++)
            if(d_b[i] > 0){
                //面が三角形
                if(ns[i][3] < 0){
                    
                    Vec edge0 = sld.vertexes[ns[i][1]] - sld.vertexes[ns[i][0]],
                    edge1 = sld.vertexes[ns[i][2]] - sld.vertexes[ns[i][1]],
                    edge2 = sld.vertexes[ns[i][0]] - sld.vertexes[ns[i][2]];
                    
                    if(dot(cross(sld.normals[i], edge0), pt - sld.vertexes[ns[i][0]]) > 0 &&
                       dot(cross(sld.normals[i], edge1), pt - sld.vertexes[ns[i][1]]) > 0 &&
                       dot(cross(sld.normals[i], edge2), pt - sld.vertexes[ns[i][2]]) > 0)
                        return d_b[i];
                }
                //面が四角形
                else{
                    Vec edge0 = sld.vertexes[ns[i][1]] - sld.vertexes[ns[i][0]],
                    edge1 = sld.vertexes[ns[i][2]] - sld.vertexes[ns[i][1]],
                    edge2 = sld.vertexes[ns[i][3]] - sld.vertexes[ns[i][2]],
                    edge3 = sld.vertexes[ns[i][0]] - sld.vertexes[ns[i][3]];
                    
                    if(dot(cross(sld.normals[i], edge0), pt - sld.vertexes[ns[i][0]]) < 0 &&
                       dot(cross(sld.normals[i], edge1), pt - sld.vertexes[ns[i][1]]) < 0 &&
                       dot(cross(sld.normals[i], edge2), pt - sld.vertexes[ns[i][2]]) < 0 &&
                       dot(cross(sld.normals[i], edge3), pt - sld.vertexes[ns[i][3]]) < 0 )
                        return d_b[i];
                }
            }
        d = kINF;
        
        //辺に一番近い場合
        vector<Vec> edges = get_edges(sld);
        int xxx;
        switch (sld.kind) {
            case Tetra:
                xxx = 3;
                break;
            case Pyram:
                xxx = 4;
                break;
            case Prism:
                xxx = 6;
                break;
            case Hexa:
                xxx = 8;
                break;
        }
        for(int i = 0; i < get_edge_n(sld.kind); i ++){
            Vec pt_ = pt - sld.vertexes[i%xxx];
            float dot_ = dot(edges[i], pt_);
            if(dot_ > 0 && dot_ < edges[i].length_squared())
                d = min(d ,cross(pt_, edges[i]).length_squared()/edges[i].length_squared());
        }
        
        //点に一番近い場合
        for(int i = 0; i < sld.vertexes.size(); i ++ )
            d = min(d, (sld.vertexes[i] - pt ).length_squared());
        
        return sqrt(d);
    }
    
    //立体からの距離
    static float getDistanceSldPt(const Vec &pt, const Solid &sld){
        
        static const int tetra_face[4][4] = { {0,1,2,-1}, {1,3,2,-1}, {0,2,3,-1}, {0,3,1,-1} };
        static const int pyram_face[5][4] = {{}};
        static const int prism_face[5][4] =
        { {0,1,2,-1}, {3,4,5,-1}, {0,3,5,1}, {0,2,4,3}, {1,5,4,2} };
        static const int hexa_face[6][4] =
        { {0,1,2,3}, {4,5,6,7}, {0,4,7,1}, {1,7,6,2}, {2,6,5,3}, {0,3,5,4} };
        
        switch (sld.kind) {
            case Tetra:
                return disntace_pt_sld(pt,sld,tetra_face);
                break;
            case Pyram:
                return disntace_pt_sld(pt, sld, pyram_face);
                break;
            case Prism:
                return disntace_pt_sld(pt,sld,prism_face);
                break;
            case Hexa:
                return disntace_pt_sld(pt,sld,hexa_face);
                break;
        }
        
        return 0;
    }
    
}

/**
 * 多面体データからVolumeDataへの変換
 * @param w Width
 * @param h Height
 * @retval true 変換成功
 * @retval false 変換失敗
 */
bool SolidToDFVolume::ToVolume(int w, int h, int d) {
    
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
    
    size_t dim[3] = { w, h, d };
    
    for (int i = 0; i < m_solid->Index()->GetNum(); i++) {
        float *vertices_f =  position + 3 * m_solid->Type() * i;
        float bmax_[3], bmin_[3];
        
        bmin_[0] = bmax_[0] = vertices_f[0];
        bmin_[1] = bmax_[1] = vertices_f[1];
        bmin_[2] = bmax_[2] = vertices_f[2];
        
        for (int i = 1; i < m_solid->Type(); i++) {
            
            bmin_[0] = (std::min)(bmin_[0], vertices_f[3*i+0]);
            bmin_[1] = (std::min)(bmin_[1], vertices_f[3*i+1]);
            bmin_[2] = (std::min)(bmin_[2], vertices_f[3*i+2]);
            
            bmax_[0] = (std::max)(bmax_[0], vertices_f[3*i+0]);
            bmax_[1] = (std::max)(bmax_[1], vertices_f[3*i+1]);
            bmax_[2] = (std::max)(bmax_[2], vertices_f[3*i+2]);
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
 * 多面体データ設定
 * @param solid 変換対象の多面体データ
 * @retval 1 変換成功
 * @retval 0 変換失敗
 */
int SolidToDFVolume::Create(BufferSolidData *solid) {
    if (!solid) {
        return 0;
    }
    
    if (solid->Position()->GetNum() < 1) {
        return 0;
    }
    
    m_solid = solid; // just copy a pointer.
    
    return 1;
}

/**
 * VolumeDataへの参照取得1
 * @retval m_volume VolumeDataへの参照
 */
BufferVolumeData* SolidToDFVolume::VolumeData()
{
    return m_volume;
}

/// コンストラクタ
SolidToDFVolume::SolidToDFVolume() : m_volume(0), m_solid(0) {
}

/// デストラクタ
SolidToDFVolume::~SolidToDFVolume() {
}
