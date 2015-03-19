/**
 * @file BufferVolumeData.cpp
 * BufferVolumeDataクラス
 */
#include "BufferVolumeData.h"
#include "Buffer.h"

#include <vector>
#include <algorithm>

namespace {

inline float remap(float x, const float *table, int n) {
  int idx = x * n;
  idx = std::max(std::min(n - 1, idx), 0);

  return table[idx];
}

} // namespace

/**
 * BufferVolumeDataクラス
 */
class BufferVolumeData::Impl
{
private:
    int m_dim[3];
    int m_comp;
    bool m_isNonUniform;
    RefPtr<FloatBuffer>  m_buffer;
    RefPtr<FloatBuffer>  m_spacingX;
    RefPtr<FloatBuffer>  m_spacingY;
    RefPtr<FloatBuffer>  m_spacingZ;
    
public:
    
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// デストラクタ
    ~Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    Impl(BufferVolumeData* inst)
    {
        this->m_dim[0] = inst->Width();
        this->m_dim[1] = inst->Height();
        this->m_dim[2] = inst->Depth();
        this->m_comp   = inst->Component();
        this->m_buffer = inst->Buffer();
        this->m_isNonUniform = inst->NonUniform();
        this->m_spacingX = inst->SpacingX();
        this->m_spacingY = inst->SpacingY();
        this->m_spacingZ = inst->SpacingZ();
    }

    /**
     * BufferVolumeDataの作成
     * @param w Widthサイズ
     * @param h Heightサイズ
     * @param d Depthサイズ
     * @param component component数
     */
    void Create(int w, int h, int d, int component)
    {
        this->m_dim[0] = w;
        this->m_dim[1] = h;
        this->m_dim[2] = d;
        this->m_comp = component;
        FloatBuffer* buf = new FloatBuffer();
        FloatBuffer* spacingX = new FloatBuffer();
        FloatBuffer* spacingY = new FloatBuffer();
        FloatBuffer* spacingZ = new FloatBuffer();
        buf->Create(w * h * d * component);
        this->m_buffer = buf;
        spacingX->Create(w);
        spacingY->Create(h);
        spacingZ->Create(d);
        this->m_spacingX = spacingX;
        this->m_spacingY = spacingY;
        this->m_spacingZ = spacingZ;
        this->m_isNonUniform = false;
    }
    
    /// メンバクリア
    void Clear()
    {
        m_dim[0] = m_dim[1] = m_dim[2] = 0;
        m_comp   = 0;
        m_buffer = 0;
        m_isNonUniform = false;
    }
    
    /// デバッグ用
    void print()
    {
    }

    /**
     * Width値取得
     * @return Width値
     */
    int Width() {
        return m_dim[0];
    }

    /**
     * Height値取得
     * @return Height値
     */
    int Height() {
        return m_dim[1];
    }

    /**
     * Depth値取得
     * @return Depth値
     */
    int Depth() {
        return m_dim[2];
    }

    /**
     * Component数取得
     * @return Component数
     */
    int Component() {
        return m_comp;
    }

    /**
     * ボリュームバッファ取得
     * @return FloatBufferボリュームバッファへの参照
     */
    FloatBuffer *Buffer() {
        return m_buffer;
    }

    /**
     * NonUniformフラグ取得
     * @return NonUniformフラグ値
     */
    bool NonUniform() {
        return m_isNonUniform;
    }
    
    FloatBuffer* SpacingX() {
        return m_spacingX;
    }
    
    FloatBuffer* SpacingY() {
        return m_spacingY;
    }
    
    FloatBuffer* SpacingZ() {
        return m_spacingZ;
    }
    /**
     * NonUniformフラグ取得
     * @param ret サンプルされたボリュームバッファ値
     * @param x X位置
     * @param y Y位置
     * @param z Z位置
     */
    void Sample(float* ret, float x, float y, float z) {

        float xx = xx;
        float yy = yy;
        float zz = zz;

        if (m_isNonUniform) {

            // remap coordinate.

            if (SpacingX()->GetNum() > 0) {
                xx = remap(xx, static_cast<const float*>(SpacingX()->GetBuffer()), SpacingX()->GetNum());
            }

            if (SpacingX()->GetNum() > 0) {
                yy = remap(yy, static_cast<const float*>(SpacingY()->GetBuffer()), SpacingY()->GetNum());
            }

            if (SpacingX()->GetNum() > 0) {
                zz = remap(zz, static_cast<const float*>(SpacingZ()->GetBuffer()), SpacingZ()->GetNum());
            }

        }

        size_t ix = std::min(std::max((size_t)(xx * Width()), (size_t)(Width()-1)), (size_t)0);
        size_t iy = std::min(std::max((size_t)(yy * Height()), (size_t)(Height()-1)), (size_t)0);
        size_t iz = std::min(std::max((size_t)(zz * Depth()), (size_t)(Depth()-1)), (size_t)0);

        size_t idx = Component() * (iz * Width() * Height() + iy * Width() + ix);

        const float* buf = static_cast<const float*>(m_buffer->GetBuffer());
        for (size_t c = 0; c < Component(); c++) {
            ret[c] = buf[idx + c];
        }

    }
};

/// constructor
BufferVolumeData::BufferVolumeData()
{
    m_imp = new BufferVolumeData::Impl();
}

/// constructor
BufferVolumeData::BufferVolumeData(BufferVolumeData* inst)
{
    m_imp = new BufferVolumeData::Impl(inst);
}

/// destructor
BufferVolumeData::~BufferVolumeData()
{
    delete m_imp;
}

void BufferVolumeData::Create(int w, int h, int d, int component)
{
    m_imp->Create(w, h, d, component);
}

void BufferVolumeData::Clear()
{
    m_imp->Clear();
}

void BufferVolumeData::print()
{
    m_imp->print();
}

int BufferVolumeData::Width() const {
    return m_imp->Width();
}

int BufferVolumeData::Height() const {
    return m_imp->Height();
}

int BufferVolumeData::Depth() const {
    return m_imp->Depth();
}

int BufferVolumeData::Component() const {
    return m_imp->Component();
}

FloatBuffer *BufferVolumeData::Buffer() const {
    return m_imp->Buffer();
}

bool BufferVolumeData::NonUniform() {
    return m_imp->NonUniform();
}

FloatBuffer* BufferVolumeData::SpacingX() {
    return m_imp->SpacingX();
}

FloatBuffer* BufferVolumeData::SpacingY() {
    return m_imp->SpacingY();
}

FloatBuffer* BufferVolumeData::SpacingZ() {
    return m_imp->SpacingZ();
}

void BufferVolumeData::Sample(float* ret, float x, float y, float z) {
    m_imp->Sample(ret, x, y, z);
}

