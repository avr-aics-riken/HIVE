/**
 * @file BufferSolidData.cpp
 * BufferSolidDataクラス
 */
#include "BufferSolidData.h"
#include "Buffer.h"
#include <vector>

/**
 * BufferSolidDataクラス
 */
class BufferSolidData::Impl
{
private:
    RefPtr<Vec3Buffer >  m_pos;
    RefPtr<UintBuffer >  m_index;
    SolidType            m_type;

public:
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    Impl(BufferSolidData* inst)
    {
        this->m_pos      = inst->Position();
        this->m_index    = inst->Index();
        this->m_type     = inst->Type();
    }
    
    /// デストラクタ
    ~Impl()
    {
        m_pos      = 0;
        m_index    = 0;
    }

    /**
     * BufferSolidDataの作成
     * @param solidType Type of the solid
     * @param vertexnum 作成頂点数
     * @param indexnum  作成index数
     */
    void Create(SolidType type, int vertexnum, int indexnum)
    {
        m_pos->Create(vertexnum);
        m_index->Create(indexnum);
        m_type = type;
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

    SolidType Type() const { return m_type; }
};

/// constructor
BufferSolidData::BufferSolidData() : BufferData(TYPE_SOLID)
{
    m_imp = new BufferSolidData::Impl();
}

/// constructor
BufferSolidData::BufferSolidData(BufferSolidData* inst) : BufferData(TYPE_SOLID)
{
    m_imp = new BufferSolidData::Impl(inst);
}

/// destructor
BufferSolidData::~BufferSolidData()
{
    delete m_imp;
}

BufferSolidData* BufferSolidData::CreateInstance()
{
    return new BufferSolidData();
}

/**
 * BufferSolidDataの作成
 * @param type Type of the solid
 * @param vertexnum 作成頂点数
 * @param indexnum  作成index数
 */
void BufferSolidData::Create(SolidType type, int vertexnum, int indexnum)
{
    m_imp->Create(type, vertexnum, indexnum);
}

/// メンバクリア
void BufferSolidData::Clear()
{
    m_imp->Clear();
}

/// デバッグ用
void BufferSolidData::print()
{
    m_imp->print();
}

/**
 * 頂点バッファ取得
 * @return Vec3Buffer頂点バッファへの参照
 */
Vec3Buffer  *BufferSolidData::Position() { return m_imp->Position(); }
/**
 * Indexバッファ取得
 * @return Indexバッファへの参照
 */
UintBuffer  *BufferSolidData::Index()    { return m_imp->Index();    }

BufferSolidData::SolidType  BufferSolidData::Type() const    { return m_imp->Type();    }
