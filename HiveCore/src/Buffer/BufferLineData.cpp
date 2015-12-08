/**
 * @file BufferLineData.cpp
 * BufferLineDataクラス
 */
#include "BufferLineData.h"
#include "Buffer.h"
#include <vector>

/**
 * BufferLineDataクラス
 */
class BufferLineData::Impl
{
private:
    RefPtr<Vec3Buffer>  m_pos;
    RefPtr<FloatBuffer> m_radius;
    RefPtr<FloatBuffer> m_mat;
    RefPtr<UintBuffer>  m_index;

public:
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    Impl(BufferLineData* inst)
    {
        this->m_pos    = inst->Position();
        this->m_mat    = inst->Material();
        this->m_radius = inst->Radius();
        this->m_index  = inst->Index();
    }

    /// デストラクタ
    ~Impl()
    {
        m_pos    = 0;
        m_mat    = 0;
        m_radius = 0;
        m_index  = 0;
    }
    
    /// メンバクリア
    void Clear()
    {
        m_pos    = new Vec3Buffer();
        m_mat    = new FloatBuffer();
        m_radius = new FloatBuffer();
        m_index  = new UintBuffer();
    }
    
    
    /**
     * BufferLineDataの作成
     * @param vertexnum 作成頂点数
     * @param indexnum  作成index数
     * @param useRadius 半径指定
     */
    void Create(int vertexnum, int indexnum, bool useRadius)
    {
       m_pos->Create(vertexnum);
        m_mat->Create(vertexnum);
        if (useRadius)
            m_radius->Create(vertexnum);
        if (indexnum)
            m_index->Create(indexnum);
    }

    /**
     * 頂点バッファ取得
     * @return Vec3Buffer頂点バッファへの参照
     */
    Vec3Buffer* Position() {
        return m_pos;
    }

    /**
     * マテリアルIDバッファ取得
     * @return FloatBufferマテリアルIDバッファへの参照
     */
    FloatBuffer* Material() {
        return m_mat;
    }

    /**
     * 半径バッファ取得
     * @return FloatBuffer半径バッファへの参照
     */
    FloatBuffer* Radius() {
        return m_radius;
    }

    /**
     * Indexバッファ取得
     * @return Indexバッファへの参照
     */
    UintBuffer* Index() {
        return m_index;
    }
    
    /**
     * 頂点バッファ取得
     * @return Vec3Buffer頂点バッファへの参照
     */
    Vec3Buffer* Position() const {
        return m_pos;
    }
    
    /**
     * マテリアルIDバッファ取得
     * @return FloatBufferマテリアルIDバッファへの参照
     */
    FloatBuffer* Material() const {
        return m_mat;
    }
    
    /**
     * 半径バッファ取得
     * @return FloatBuffer半径バッファへの参照
     */
    FloatBuffer* Radius() const {
        return m_radius;
    }
    
    /**
     * Indexバッファ取得
     * @return Indexバッファへの参照
     */
    UintBuffer* Index() const {
        return m_index;
    }
};

/// constructor
BufferLineData::BufferLineData() : BufferData(TYPE_LINE)
{
    m_imp = new BufferLineData::Impl();
}

/// destructor
BufferLineData::~BufferLineData()
{
    delete m_imp;
}

/// constructor
BufferLineData::BufferLineData(BufferLineData* inst) : BufferData(TYPE_LINE)
{
    m_imp = new BufferLineData::Impl(inst);
}


BufferLineData* BufferLineData::CreateInstance()
{
    return new BufferLineData();
}



/// メンバクリア
void BufferLineData::Clear()
{
    m_imp->Clear();
}

/**
 * BufferLineDataの作成
 * @param vertexnum 作成頂点数
 * @param indexnum  作成index数
 * @param useRadius 半径指定
 */
void BufferLineData::Create(int vertexnum, int indexnum, bool useRadius)
{
    m_imp->Create(vertexnum, indexnum, useRadius);
}

/**
 * 頂点バッファ取得
 * @return Vec3Buffer頂点バッファへの参照
 */
Vec3Buffer* BufferLineData::Position() {
    return m_imp->Position();
}

/**
 * マテリアルIDバッファ取得
 * @return FloatBufferマテリアルIDバッファへの参照
 */
FloatBuffer* BufferLineData::Material() {
    return m_imp->Material();
}

/**
 * 半径バッファ取得
 * @return FloatBuffer半径バッファへの参照
 */
FloatBuffer* BufferLineData::Radius() {
    return m_imp->Radius();
}

/**
 * Indexバッファ取得
 * @return Indexバッファへの参照
 */
UintBuffer* BufferLineData::Index() {
    return m_imp->Index();
}

/**
 * 頂点バッファ取得
 * @return Vec3Buffer頂点バッファへの参照
 */
Vec3Buffer* BufferLineData::Position() const {
    return m_imp->Position();
}

/**
 * マテリアルIDバッファ取得
 * @return FloatBufferマテリアルIDバッファへの参照
 */
FloatBuffer* BufferLineData::Material() const {
    return m_imp->Material();
}

/**
 * 半径バッファ取得
 * @return FloatBuffer半径バッファへの参照
 */
FloatBuffer* BufferLineData::Radius() const {
    return m_imp->Radius();
}

/**
 * Indexバッファ取得
 * @return Indexバッファへの参照
 */
UintBuffer* BufferLineData::Index() const {
    return m_imp->Index();
}


