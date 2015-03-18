#include "BufferImageData.h"
#include "Buffer.h"
#include <vector>

class BufferImageData::Impl
{
private:
    int m_width;
    int m_height;
    FORMAT m_format;
    RefPtr<FloatBuffer> m_floatbuffer;
    RefPtr<ByteBuffer> m_bytebuffer;
    float m_gamma;
    float m_exposure;
    
public:
    /// コンストラクタ
    Impl()
    {
        Clear();
    }
    
    /// コンストラクタ
    Impl(BufferImageData* data)
    {
        Clear();
        m_width = data->Width();
        m_height = data->Height();
        m_format = data->Format();
        m_floatbuffer = data->FloatImageBuffer();
        m_bytebuffer = data->ImageBuffer();
        m_gamma = data->Gamma();
        m_exposure = data->Exposure();
    }
    
    
    /**
     * ImageBuffer作成
     * @param type 作成するBufferType
     * @param w Width
     * @param h Height
     */
    void Create(FORMAT type, int w, int h)
    {
        m_width = w;
        m_height = h;
        m_format = type;
        if (type == RGB8) {
            m_bytebuffer = new ByteBuffer();
            m_bytebuffer->Create(w * h * 3);
        } else if (type == RGBA8) {
            m_bytebuffer = new ByteBuffer();
            m_bytebuffer->Create(w * h * 4);
        } else if (type == R32F) {
            m_floatbuffer = new FloatBuffer();
            m_floatbuffer->Create(w * h);
        } else if (type == RGBA32F) {
            m_floatbuffer = new FloatBuffer();
            m_floatbuffer->Create(w * h * 4);
        }
    }

    /// メンバクリア
    void Clear()
    {
        m_width = 0;
        m_height = 0;
        m_format = INVALID;
        m_floatbuffer = NULL;
        m_bytebuffer = NULL;
        m_gamma = 1.0f;
        m_exposure = 1.0f;
    }
    
    
    /**
     * Width取得
     * @return Width値
     */
    int Width() const
    {
        return m_width;
    }
    
    /**
     * Height取得
     * @return Height値
     */
    int Height() const
    {
        return m_height;
    }
    
    
    /**
     * FORMAT取得
     * @return FORMATパラメータ
     */
    FORMAT Format() const
    {
        return m_format;
    }

    /**
     * 作成したバッファサイズ取得
     * @return バッファバイト数
     */
    int Bytes() const
    {
        int wxh = m_width * m_height;
        if (m_format == RGBA8) {
            return wxh * 4;
        } else if (m_format == RGB8) {
            return wxh * 3;
        } else if (m_format == R32F) {
            return wxh * 4;
        } else if (m_format == RGBA32F) {
            return wxh * 4 * 4;
        }
        return -1;
    }

    /**
     * 浮動小数点バッファ参照取得
     * @return 浮動小数点バッファへの参照
     */
    FloatBuffer* FloatImageBuffer()
    {
        return m_floatbuffer;
    }

    /**
     * 浮動小数点バッファ参照取得
     * @return 浮動小数点バッファへの参照
     */
    FloatBuffer* FloatImageBuffer() const
    {
        return m_floatbuffer;
    }

    /**
     * Byteバッファ参照取得
     * @return Byteバッファ参照への参照
     */
    ByteBuffer* ImageBuffer()
    {
        return m_bytebuffer;
    }

    /**
     * Byteバッファ参照取得
     * @return Byteバッファ参照への参照
     */
    ByteBuffer* ImageBuffer() const
    {
        return m_bytebuffer;
    }

    /**
     * Gamma値取得
     * @return Gamma値
     */
    float Gamma() const
    {
        return m_gamma;
    }

    /**
     * Exposure値取得
     * @return Exposure値
     */
    float Exposure() const
    {
        return m_exposure;
    }

    /**
     * Gamma値設定
     * @param Gamma値
     */
    void SetGamma(float gamma)
    {
        m_gamma = gamma;
    }

    /**
     * Exposure値設定
     * @param Exposure値
     */
    void SetExposure(float exposure)
    {
        m_exposure = exposure;
    }
};

/// constructor
BufferImageData::BufferImageData()
{
    m_imp = new Impl();
}

/// constructor
BufferImageData::BufferImageData(BufferImageData* inst)
{
    m_imp = new Impl(inst);
}

/// destructor
BufferImageData::~BufferImageData()
{
    delete m_imp;
}

void BufferImageData::Create(FORMAT format, int w, int h)
{
    m_imp->Create(format, w, h);
}

void BufferImageData::Clear()
{
    m_imp->Clear();
}

int BufferImageData::Width() const
{
    return m_imp->Width();
}

int BufferImageData::Height() const
{
    return m_imp->Height();
}

BufferImageData::FORMAT BufferImageData::Format() const
{
    return m_imp->Format();
}

int BufferImageData::Bytes() const
{
    return m_imp->Bytes();
}

FloatBuffer* BufferImageData::FloatImageBuffer()
{
    return m_imp->FloatImageBuffer();
}

FloatBuffer* BufferImageData::FloatImageBuffer() const
{
    return m_imp->FloatImageBuffer();
}

ByteBuffer* BufferImageData::ImageBuffer()
{
    return m_imp->ImageBuffer();
}

ByteBuffer* BufferImageData::ImageBuffer() const
{
    return m_imp->ImageBuffer();
}

float BufferImageData::Gamma() const
{
    return m_imp->Gamma();
}

float BufferImageData::Exposure() const
{
    return m_imp->Exposure();
}

void BufferImageData::SetGamma(float gamma)
{
    m_imp->SetGamma(gamma);
}

void BufferImageData::SetExposure(float exposure)
{
    m_imp->SetExposure(exposure);
}

