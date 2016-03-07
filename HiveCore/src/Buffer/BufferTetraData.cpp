/**
 * @file BufferTetraData.cpp
 * BufferTetraDataクラス
 */
#include "BufferTetraData.h"
#include "Buffer.h"
#include <vector>

/**
 * BufferTetraDataクラス
 */
class BufferTetraData::Impl
{
private:
    RefPtr<Vec3Buffer >  m_pos;
    RefPtr<UintBuffer >  m_index;
public:
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    Impl(BufferTetraData* inst)
    {
        this->m_pos      = inst->Position();
        this->m_index    = inst->Index();
    }
    
    /// デストラクタ
    ~Impl()
    {
        m_pos      = 0;
        m_index    = 0;
    }

    /**
     * BufferTetraDataの作成
     * @param vertexnum 作成頂点数
     * @param indexnum  作成index数
     */
    void Create(int vertexnum, int indexnum)
    {
        m_pos->Create(vertexnum);
        m_index->Create(indexnum);
    }
    
    /// メンバクリア
    void Clear()
    {
        m_pos      = new Vec3Buffer();
        m_index    = new UintBuffer();
    }
    
    /// デバッグ用
    void print()
    {
        /*	printf("Position() %08p\n", m_pos.Get()      );
         printf("Index()    %08X\n", m_index.Get()    );*/
    }
    
    /**
     * 頂点バッファ取得
     * @return Vec3Buffer頂点バッファへの参照
     */
    Vec3Buffer  *Position() { return m_pos;      }

    /**
     * Indexバッファ取得
     * @return Indexバッファへの参照
     */
    UintBuffer  *Index()    { return m_index;    }
};

/// constructor
BufferTetraData::BufferTetraData() : BufferData(TYPE_TETRA)
{
    m_imp = new BufferTetraData::Impl();
}

/// constructor
BufferTetraData::BufferTetraData(BufferTetraData* inst) : BufferData(TYPE_TETRA)
{
    m_imp = new BufferTetraData::Impl(inst);
}

/// destructor
BufferTetraData::~BufferTetraData()
{
    delete m_imp;
}


BufferTetraData* BufferTetraData::CreateInstance()
{
    return new BufferTetraData();
}

/**
 * BufferTetraDataの作成
 * @param vertexnum 作成頂点数
 * @param indexnum  作成index数
 */
void BufferTetraData::Create(int vertexnum, int indexnum)
{
    m_imp->Create(vertexnum, indexnum);
}

/// メンバクリア
void BufferTetraData::Clear()
{
    m_imp->Clear();
}

/// デバッグ用
void BufferTetraData::print()
{
    m_imp->print();
}

/**
 * 頂点バッファ取得
 * @return Vec3Buffer頂点バッファへの参照
 */
Vec3Buffer  *BufferTetraData::Position() { return m_imp->Position(); }
/**
 * Indexバッファ取得
 * @return Indexバッファへの参照
 */
UintBuffer  *BufferTetraData::Index()    { return m_imp->Index();    }

