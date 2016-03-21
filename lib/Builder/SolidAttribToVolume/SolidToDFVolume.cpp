/**
 * @file SolidToDFVolume.cpp
 * 点データからVolumeDataへ変換するモジュール
 */
#include "SolidToDFVolume.h"
#include "../Core/vxmath.h"
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
        
        Solid (float *verts_, int type_);
    };
    
    typedef unsigned int sld_id_;
    
    class BVH{
        
        struct BVHNode{
            VX::Math::vec3 start, end;
            unsigned int children[2];
            
            int leaf;
            
            BVHNode() : leaf(-1) {}
            
        };
        void getBoudinBox(const Solid &solid, int solid_id, BVHNode &node){
            
            node.start = solid.verts[0];
            node.end = solid.verts[0];
            
            for (int i = 1; i < solid.verts.size(); i ++){
                node.start.x = std::min(solid.verts[i].x, node.start.x);
                node.start.y = std::min(solid.verts[i].y, node.start.y);
                node.start.z = std::min(solid.verts[i].z, node.start.z);
                
                node.end.x = std::max(solid.verts[i].x, node.end.x);
                node.end.y = std::max(solid.verts[i].y, node.end.y);
                node.end.z = std::max(solid.verts[i].z, node.end.z);
            }
            
            const float kEPS = 1e-5;
            
            node.start = node.start - VX::Math::vec3(kEPS, kEPS, kEPS);
            node.end = node.end + VX::Math::vec3(kEPS, kEPS, kEPS);
            
            node.leaf = solid_id;
            
        }
        inline float getDistance(BVHNode &bvh_node, const VX::Math::vec3 &point){
            
            float d = -1e16;
            float d_b = 0;
            for (int i = 0; i < 3; i++) {
                float d0 = std::max( bvh_node.start[i] - point[i], point[i] - bvh_node.end[i]);
                
                if(d0 > 0)
                    d_b += d0 * d0;
                else if(d <= d0) d = d0;
            }
            if(d_b == 0) return d;
            return sqrt(d_b);
            
        }
        
        std::vector<Solid> solids;
        std::vector<BVHNode> tree;
        
        void getDf(const VX::Math::vec3 &point, float &last_d);
        
        void nodeFit(unsigned int node_id){
            if(tree[node_id].leaf == -1){
                for (int i = 0; i < 3; i++ ){
                    tree[node_id].start[i] = std::min(tree[tree[node_id].children[0]].start[i], tree[tree[node_id].children[1]].start[i]);
                    tree[node_id].end[i] = std::max(tree[tree[node_id].children[0]].end[i], tree[tree[node_id].children[1]].end[i]);
                }
            }else
                getBoudinBox(solids[node_id], tree[node_id].leaf, tree[node_id]);
            
        }

        
        float getAabbSurface(std::vector<sld_id_> slds){
            using namespace VX::Math;
            const float kINF = 1e16;
            vec3 start(kINF,kINF,kINF), end(-kINF,-kINF,-kINF);
            for(int i = 0; i < slds.size(); i++)
                for(int j = 0; j < solids[i].type; j++)
                    for(int axis = 0; axis < 3; axis++){
                        start[axis] = min(start[axis], solids[slds[i]].verts[j][axis]);
                        end[axis] = max(end[axis], solids[slds[i]].verts[j][axis]);
                    }
            vec3 box = end - start;
            return 2.f *( box.x * box.y + box.z * box.y + box.x * box.z);
        }
        
        void generateBvhTree (const std::vector<Solid> &solids){
            this->solids = solids;
            tree.resize(1);
            
            for(int i = 0; i < solids.size(); i ++)
                for (int j = 1; j < solids[i].verts.size(); j ++){
                    tree[0].start.x = std::min(solids[i].verts[j].x, tree[0].start.x);
                    tree[0].start.y = std::min(solids[i].verts[j].y, tree[0].start.y);
                    tree[0].start.z = std::min(solids[i].verts[j].z, tree[0].start.z);
                    
                    tree[0].end.x = std::max(solids[i].verts[j].x, tree[0].end.x);
                    tree[0].end.y = std::max(solids[i].verts[j].y, tree[0].end.y);
                    tree[0].end.z = std::max(solids[i].verts[j].z, tree[0].end.z);
                }
            
            std::vector<unsigned int> all(solids.size());
            for(int i = 0; i < all.size(); i ++)
                all[i] = i;
            
            makeBvhNode(all, 0, 0);
            
        }
        
        typedef float center_;
        
        inline center_ med3(center_ x, center_ y, center_ z)
        /* x, y, z の中間値を返す */
        {
            if (x < y)
                if (y < z) return y;
                else if (z < x) return x;
                else return z;
                else if (z < y) return y;
                else if (x < z) return x;
                else return z;
        }
        
        void quicksort(std::vector<center_> &centers, std::vector<sld_id_> &slds, int left, int right)
        /* クイックソート
         * left  : ソートするデータの開始位置
         * right : ソートするデータの終了位置
         */
        {
            if (left < right) {
                int i = left, j = right;
                center_ tmp_c, pivot = med3(centers[i], centers[i + (j - i) / 2], centers[j]);
                sld_id_ tmp_s;
                //(i+j)/2ではオーバーフローしてしまう
                while (1) { /* a[] を pivot 以上と以下の集まりに分割する */
                    while (centers[i] < pivot) i++; /* a[i] >= pivot となる位置を検索 */
                    while (pivot < centers[j]) j--; /* a[j] <= pivot となる位置を検索 */
                    if (i >= j) break;
                    tmp_c = centers[i]; centers[i] = centers[j]; centers[j] = tmp_c; // a[i],a[j] を交換
                    tmp_s = slds[i]; slds[i] = slds[j]; slds[j] = tmp_s; // slds[i],slds[j] を交換
                    
                    i++; j--;
                }
                quicksort(centers, slds, left, i - 1);  /* 分割した左を再帰的にソート */
                quicksort(centers, slds, j + 1, right); /* 分割した右を再帰的にソート */
            }
        }
        
        inline float getCenter(const Solid &sld, int axis){
            float center = 0;
            int n = (int)sld.verts.size();
            for (int i = 0; i < n; i++)
                center += sld.verts[i][axis];
            return center / n;
        }
        
        void makeBvhNode(std::vector<sld_id_> &solid_ids, const unsigned int nowNode, const unsigned int tree_depth){
            
            if(solid_ids.size() < 2){
                getBoudinBox(solids[solid_ids[0]], solid_ids[0], tree[nowNode]);
                return;
            }
            
            std::vector<sld_id_> next_right;
            std::vector<sld_id_> next_left;
            
            std::vector<sld_id_> nr_buffer;
            std::vector<sld_id_> nl_buffer;
            int best_axis = 0;
            float best_face = 0;
            float best_surface = 1e16;
            
            for (int i = tree_depth; i < tree_depth + 3; i++) {
                int axis = tree_depth%3;
                
                std::vector<center_> centers(solid_ids.size());
                for (int n = 0; n < centers.size(); n++)
                    centers[n] = getCenter(solids[solid_ids[n]], axis);
                
                quicksort(centers, solid_ids, 0, (int)centers.size()-1);
                
                nr_buffer = std::vector<sld_id_>( &solid_ids[0], &solid_ids[0] + solid_ids.size()/2 );
                nl_buffer = std::vector<sld_id_>( &solid_ids[0] + solid_ids.size()/2,
                                            &solid_ids[0] + solid_ids.size());
                
                if(best_surface > getAabbSurface(nr_buffer) + getAabbSurface(nl_buffer)){
                    best_axis = axis;
                    best_face = (centers[centers.size()/2-1] + centers[centers.size()/2]) / 2;
                    next_left = nl_buffer;
                    next_right = nr_buffer;
                }
            }
            
        MakeNextNode:
            tree.push_back(BVHNode());
            tree.push_back(BVHNode());
            
            tree[tree.size()-2].start = tree[nowNode].start;
            tree[tree.size()-2].end = tree[nowNode].end;
            tree[tree.size()-2].end[best_axis] = best_face;
            
            tree[tree.size()-1].start = tree[nowNode].start;
            tree[tree.size()-1].end = tree[nowNode].end;
            tree[tree.size()-1].start[best_axis] = best_face;
            
            //    child0.mother_bvh_id = nowNode;
            //    child1.mother_bvh_id = nowNode;
            
            tree[nowNode].children[0] = (unsigned int)tree.size()-2;
            tree[nowNode].children[1] = (unsigned int)tree.size()-1;
            
            makeBvhNode(next_right, tree[nowNode].children[0], tree_depth+1);
            makeBvhNode(next_left, tree[nowNode].children[1], tree_depth+1);
            
            nodeFit(nowNode);
            
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
    
    Solid::Solid(float *verts_, int type_) : type((SolidType)type_){
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
                edges[7] = verts[4]-verts[1];
                edges[8] = verts[5]-verts[2];
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
                break;
        }
        normals.resize(face_n(type));
        
        static const int faces[][6][4] = {
            {{}}, {{}}, {{}}, {{}},        // 0,1,2,3
            {{0,2,1,-1}, {1,2,3,-1}, {0,3,2,-1}, {0,1,3,-1} },  //tetra
            {{0,3,2,1}, {0,1,4,-1}, {1,2,4,-1}, {2,3,4,-1}, {0,4,3,-1} },  //pyramid
            {{0,2,1,-1}, {3,4,5,-1}, {0,3,5,2}, {0,1,4,3}, {1,2,5,4} },   //prism
            {{}},       // 7
            {{0,3,2,1}, {4,5,6,7}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {0,4,7,3}, },  //hexa
        };
        normals.resize(face_n(type));
        for (int i = 0; i < face_n(type); i ++) {
            normals[i] =  normalize(cross(verts[faces[type][i][1]]
                                          - verts[faces[type][i][0]],
                                          verts[faces[type][i][2]]
                                          - verts[faces[type][i][1]]));
        }
    }
    
    
    //立体からの距離
    inline float getDistanceSldPt(const VX::Math::vec3 &pt, const Solid &sld){
        
        static const int faces[][6][4] = {
            {{}}, {{}}, {{}}, {{}},        // 0,1,2,3
            {{0,2,1,-1}, {1,2,3,-1}, {0,3,2,-1}, {0,1,3,-1} },  //tetra
            {{0,3,2,1}, {0,1,4,-1}, {1,2,4,-1}, {2,3,4,-1}, {0,4,3,-1} },  //pyramid
            {{0,2,1,-1}, {3,4,5,-1}, {0,3,5,2}, {0,1,4,3}, {1,2,5,4} },   //prism
            {{}},       // 7
            {{0,3,2,1}, {4,5,6,7}, {0,1,5,4}, {1,2,6,5}, {2,3,7,6}, {0,4,7,3}, },  //hexa
        };
        
        using namespace VX::Math;
        float d = -1e16;
        float d_b[6];
        
        //内部にある場合
        for(int i = 0, n = face_n(sld.type); i < n; i ++){
            d_b[i] = dot(sld.normals[i],
                         pt - sld.verts[faces[sld.type][i][0]]);
            if(d < d_b[i]) d = d_b[i];
        }
        if(d < 0){
            return d;
        }
        
        //面に一番近い場合
        for(int i = 0; i < sld.normals.size(); i ++)
            if(d_b[i] > 0){
                //面が三角形
                if(faces[sld.type][i][3] < 0){
                    
                    vec3 edge0 = sld.verts[faces[sld.type][i][1]] - sld.verts[faces[sld.type][i][0]],
                    edge1 = sld.verts[faces[sld.type][i][2]] - sld.verts[faces[sld.type][i][1]],
                    edge2 = sld.verts[faces[sld.type][i][0]] - sld.verts[faces[sld.type][i][2]];
                    
                    if(dot(cross(sld.normals[i], edge0), pt - sld.verts[faces[sld.type][i][0]]) > 0 &&
                       dot(cross(sld.normals[i], edge1), pt - sld.verts[faces[sld.type][i][1]]) > 0 &&
                       dot(cross(sld.normals[i], edge2), pt - sld.verts[faces[sld.type][i][2]]) > 0)
                        return d_b[i];
                }
                //面が四角形
                else{
                    vec3 edge0 = sld.verts[faces[sld.type][i][1]] - sld.verts[faces[sld.type][i][0]],
                    edge1 = sld.verts[faces[sld.type][i][2]] - sld.verts[faces[sld.type][i][1]],
                    edge2 = sld.verts[faces[sld.type][i][3]] - sld.verts[faces[sld.type][i][2]],
                    edge3 = sld.verts[faces[sld.type][i][0]] - sld.verts[faces[sld.type][i][3]];
                    
                    if(dot(cross(sld.normals[i], edge0), pt - sld.verts[faces[sld.type][i][0]]) < 0 &&
                       dot(cross(sld.normals[i], edge1), pt - sld.verts[faces[sld.type][i][1]]) < 0 &&
                       dot(cross(sld.normals[i], edge2), pt - sld.verts[faces[sld.type][i][2]]) < 0 &&
                       dot(cross(sld.normals[i], edge3), pt - sld.verts[faces[sld.type][i][3]]) < 0 )
                        return d_b[i];
                }
            }
        d = 1e16;
        
        //辺に一番近い場合
        int xxx;
        switch (sld.type) {
            case Solid::SOLID_TETRA:
                xxx = 3;
                break;
            case Solid::SOLID_PYRAMID:
                xxx = 4;
                break;
            case Solid::SOLID_PRISM:
                xxx = 6;
                break;
            case Solid::SOLID_HEXAHEDRON:
                xxx = 8;
                break;
        }
        
        for(int i = 0; i < sld.type; i ++){
            vec3 pt_ = pt - sld.verts[i%xxx];
            float dot_ = dot(sld.edges[i], pt_);
            if(dot_ > 0 && dot_ < length(sld.edges[i]))
                d = min(d ,length(cross(pt_, sld.edges[i])) / length(sld.edges[i]));
        }
        
        //点に一番近い場合
        for(int i = 0; i < sld.verts.size(); i ++ )
            d = min(d, length(sld.verts[i] - pt));
        
        return sqrt(d);
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
                    
                }
    }
    
    for (int x = 0, y, z; x < w; x++)
        for (y = 0; y < h; y++)
            for (z = 0; z < d; z++){
                VX::Math::vec3 pt(m_bmin[0] + x, m_bmin[1] + y, m_bmin[2] + z);
                
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
int SolidToDFVolume::Create(BufferSolidData *solid, float *bmin, float *bmax){
    if (!solid) {
        return 0;
    }
    
    if (solid->Position()->GetNum() < 1) {
        return 0;
    }
    
    m_solid = solid; // just copy a pointer.
    
    m_bmin[0] = bmin[0];
    m_bmin[1] = bmin[1];
    m_bmin[2] = bmin[2];
    
    m_bmax[0] = bmax[0];
    m_bmax[1] = bmax[1];
    m_bmax[2] = bmax[2];

    
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
