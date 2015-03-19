/**
 * @file BufferMeshData.cpp
 * BufferMeshDataクラス
 */
#include "BufferMeshData.h"
#include "Buffer.h"
#include <vector>

/**
 * BufferMeshDataクラス
 */
class BufferMeshData::Impl
{
private:
    RefPtr<Vec3Buffer >  m_pos;
    RefPtr<Vec3Buffer >  m_normal;
    RefPtr<Vec2Buffer >  m_texcoord;
    RefPtr<UintBuffer >  m_index;
    RefPtr<FloatBuffer>  m_mat;
public:
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    /// @param inst メッシュデータ
    Impl(BufferMeshData* inst)
    {
        this->m_pos      = inst->Position();
        this->m_normal   = inst->Normal();
        this->m_texcoord = inst->Texcoord();
        this->m_index    = inst->Index();
        this->m_mat      = inst->Material();
    }
    
    /// デストラクタ
    ~Impl()
    {
        m_pos      = 0;
        m_normal   = 0;
        m_texcoord = 0;
        m_index    = 0;
        m_mat      = 0;
    }

    /**
     * BufferMeshDataの作成
     * @param vertexnum 作成頂点数
     * @param indexnum  作成index数
     */
    void Create(int vertexnum, int indexnum)
    {
        m_pos->Create(vertexnum);
        m_normal->Create(vertexnum);
        m_texcoord->Create(vertexnum);
        m_mat->Create(vertexnum);
        m_index->Create(indexnum);
    }
    
    /// メンバクリア
    void Clear()
    {
        m_pos      = new Vec3Buffer();
        m_normal   = new Vec3Buffer();
        m_texcoord = new Vec2Buffer();
        m_mat      = new FloatBuffer();
        m_index    = new UintBuffer();
    }
    
    /// デバッグ表示
    void print()
    {
        /*	printf("Position() %08p\n", m_pos.Get()      );
         printf("Normal()   %08X\n", m_normal.Get()   );
         printf("Texcoord() %08X\n", m_texcoord.Get() );
         printf("Index()    %08X\n", m_index.Get()    );
         printf("Material() %08X\n", m_mat.Get()      );*/
    }
    
    /// 頂点バッファを返す
    Vec3Buffer  *Position() { return m_pos;      }
    /// 法線バッファを返す
    Vec3Buffer  *Normal()   { return m_normal;   }
    /// UV座標バッファを返す
    Vec2Buffer  *Texcoord() { return m_texcoord; }
    /// インデックスバッファを返す
    UintBuffer  *Index()    { return m_index;    }
    /// マテリアルバッファを返す
    FloatBuffer *Material() { return m_mat;      }
};

/// constructor
BufferMeshData::BufferMeshData()
{
    m_imp = new BufferMeshData::Impl();
}

/// constructor
/// @param inst メッシュデータ
BufferMeshData::BufferMeshData(BufferMeshData* inst)
{
    m_imp = new BufferMeshData::Impl(inst);
}

/// destructor
BufferMeshData::~BufferMeshData()
{
    delete m_imp;
}

/**
 * BufferMeshDataの作成
 * @param vertexnum 作成頂点数
 * @param indexnum  作成index数
 */
void BufferMeshData::Create(int vertexnum, int indexnum)
{
    m_imp->Create(vertexnum, indexnum);
}

/// メンバクリア
void BufferMeshData::Clear()
{
    m_imp->Clear();
}

/// デバッグ表示
void BufferMeshData::print()
{
    m_imp->print();
}

/// 頂点バッファを返す
Vec3Buffer  *BufferMeshData::Position() { return m_imp->Position(); }
/// 法線バッファを返す
Vec3Buffer  *BufferMeshData::Normal()   { return m_imp->Normal();   }
/// UV座標バッファを返す
Vec2Buffer  *BufferMeshData::Texcoord() { return m_imp->Texcoord(); }
/// インデックスバッファを返す
UintBuffer  *BufferMeshData::Index()    { return m_imp->Index();    }
/// マテリアルバッファを返す
FloatBuffer *BufferMeshData::Material() { return m_imp->Material(); }

