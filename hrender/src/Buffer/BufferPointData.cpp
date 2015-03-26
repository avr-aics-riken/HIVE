/**
 * @file BufferPointData.cpp
 * BufferPointDataクラス
 */
#include "BufferPointData.h"
#include "Buffer.h"
#include <vector>

/**
 * BufferPointDataクラス
 */
class BufferPointData::Impl
{
private:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<FloatBuffer> m_radius;
    RefPtr<FloatBuffer> m_mat;
    
public:
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    Impl(BufferPointData* inst)
    {
        this->m_pos    = inst->Position();
        this->m_mat    = inst->Material();
        this->m_radius = inst->Radius();
    }
    
    /// デストラクタ
    ~Impl()
    {
        m_pos    = 0;
        m_mat    = 0;
        m_radius = 0;
    }
    
    /// メンバクリア
    void Clear()
    {
        m_pos    = new Vec3Buffer();
        m_mat    = new FloatBuffer();
        m_radius = new FloatBuffer();
    }

    /**
     * BufferPointDataの作成
     * @param vertexnum 作成頂点数
     */
    void Create(int vertexnum)
    {
        m_pos->Create(vertexnum);
        m_mat->Create(vertexnum);
        m_radius->Create(vertexnum);
    }
    
    /**
     * 頂点バッファ取得
     * @return Vec3Buffer 頂点バッファへの参照
     */
    Vec3Buffer* Position() {
        return m_pos;
    }
    
    /**
     * マテリアルIDバッファ取得
     * @return FloatBuffer マテリアルIDバッファへの参照
     */
    FloatBuffer* Material() {
        return m_mat;
    }
    
    /**
     * 半径バッファ取得
     * @return FloatBuffer 半径バッファへの参照
     */
    FloatBuffer* Radius() {
        return m_radius;
    }
    
    /**
     * 頂点バッファ取得
     * @return Vec3Buffer 頂点バッファへの参照
     */
    Vec3Buffer* Position() const {
        return m_pos;
    }
    
    /**
     * マテリアルIDバッファ取得
     * @return FloatBuffer* マテリアルIDバッファへの参照
     */
    FloatBuffer* Material() const {
        return m_mat;
    }
    
    /**
     * 半径バッファ取得
     * @return FloatBuffer* 半径バッファへの参照
     */
    FloatBuffer* Radius() const {
        return m_radius;
    }
};

/// constructor
BufferPointData::BufferPointData()
{
	m_type = BufferData::TYPE_POINT;
    m_imp = new BufferPointData::Impl();
}

/// constructor
BufferPointData::BufferPointData(BufferPointData* inst)
{
	m_type = BufferData::TYPE_POINT;
    m_imp = new BufferPointData::Impl(inst);
}

/// destructor
BufferPointData::~BufferPointData()
{
    delete m_imp;
}

/// メンバクリア
void BufferPointData::Clear()
{
    m_imp->Clear();
}

/**
 * BufferPointDataの作成
 * @param vertexnum 作成頂点数
 */
void BufferPointData::Create(int vertexnum)
{
    m_imp->Create(vertexnum);
}

/**
 * 頂点バッファ取得
 * @return Vec3Buffer 頂点バッファへの参照
 */
Vec3Buffer* BufferPointData::Position() {
    return m_imp->Position();
}

/**
 * マテリアルIDバッファ取得
 * @return FloatBuffer マテリアルIDバッファへの参照
 */
FloatBuffer* BufferPointData::Material() {
    return m_imp->Material();
}

/**
 * 半径バッファ取得
 * @return FloatBuffer 半径バッファへの参照
 */
FloatBuffer* BufferPointData::Radius() {
    return m_imp->Radius();
}

/**
 * 頂点バッファ取得
 * @return Vec3Buffer 頂点バッファへの参照
 */
Vec3Buffer* BufferPointData::Position() const {
    return m_imp->Position();
}

/**
 * マテリアルIDバッファ取得
 * @return FloatBuffer* マテリアルIDバッファへの参照
 */
FloatBuffer* BufferPointData::Material() const {
    return m_imp->Material();
}

/**
 * 半径バッファ取得
 * @return FloatBuffer* 半径バッファへの参照
 */
FloatBuffer* BufferPointData::Radius() const {
    return m_imp->Radius();
}


