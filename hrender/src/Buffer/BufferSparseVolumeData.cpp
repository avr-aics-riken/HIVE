/**
 * @file BufferSparseVolumeData.cpp
 * BufferSparseVolumeDataクラス
 */
#include "BufferSparseVolumeData.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

// Use SparseVolume feature from SURFACE to provide Sample() function
#include "render_prim_volume.h"
#include "render_accel_volume.h"

/**
 * BufferSparseVolumeDataクラス
 */
class BufferSparseVolumeData::Impl
{
private:
    int m_dim[3];
    int m_comp;
    std::vector<VolumeBlock> m_volumes;

    lsgl::render::SparseVolumeAccel *m_sparseVolumeAccel;
    lsgl::render::SparseVolume *m_sparseVolume;

public:
    
    /// コンストラクタ
    Impl()
    {
    }
    
    /// コンストラクタ
    Impl(BufferSparseVolumeData* inst)
    {
    }
    
    /// デストラクタ
    ~Impl()
    {
    }

    /// BufferSparseVolumeDataの作成
    void Create(int w, int h, int d, int component)
    {
    }
    
    /**
     * BufferSparseVolumeDataへのBufferVolumeDataの追加
     * @param offset_x SparseBlockオフセットX
     * @param offset_y SparseBlockオフセットY
     * @param offset_z SparseBlockオフセットZ
     * @param vol      対象のBufferVolumeData
     */
    void AddVolume(int offset_x, int offset_y, int offset_z,
                                           BufferVolumeData* vol)
    {
        // @todo { Finish implementing this function. }
        {
            // add to vol to VolumeBlock.
        }

        // Also add to SparseVolume
        {
            lsgl::render::VolumeBlock block;
            block.offset[0] = offset_x;
            block.offset[1] = offset_y;
            block.offset[2] = offset_z;

            block.extent[0] = vol->Width();
            block.extent[1] = vol->Height();
            block.extent[2] = vol->Depth();

            assert(block.offset[0] + block.extent[0] <= Width());
            assert(block.offset[1] + block.extent[1] <= Width());
            assert(block.offset[2] + block.extent[2] <= Width());

            block.id = m_volumes.size();
            block.data = reinterpret_cast<unsigned char*>(vol->Buffer()->GetBuffer()); // @fixme { Take a pointer reference. Is this OK? }

            m_sparseVolume->blocks.push_back(block);
        }


    }
    
    /// メンバクリア
    void Clear()
    {
        delete m_sparseVolumeAccel;
        delete m_sparseVolume;
    }
    
    /// デバッグ用
    void print()
    {
    }
    
    /// Width取得
    int Width()
    {
        return -1; // @todo
    }
    
    /// Height取得
    int Height()
    {
        return -1; // @todo
    }
    
    /// Depth取得
    int Depth()
    {
        return -1; // @todo
    }
    
    /// Component数取得
    int Component()
    {
        return 1; // @todo
    }
    
    const std::vector<BufferSparseVolumeData::VolumeBlock>& Buffers() const
    {
        // @todo
        static std::vector<BufferSparseVolumeData::VolumeBlock> tmp;
        return tmp;
    }
    
    std::vector<BufferSparseVolumeData::VolumeBlock>& Buffers()
    {
        // @todo
        static std::vector<BufferSparseVolumeData::VolumeBlock> tmp;
        return tmp;
    }

    void Sample(float* ret, float x, float y, float z) {

        // Up to 4 components at this time.
        assert(m_comp <= 4);
        assert(m_sparseVolumeAccel);

        double value[4];
        double position[3];

        position[0] = x * Width();
        position[1] = y * Height();
        position[2] = z * Depth();

        m_sparseVolumeAccel->Sample(value, position);

        for (int c = 0; c < m_comp; c++) {
            ret[c] = value[c];
        }

    }

    bool Build() {
        delete m_sparseVolumeAccel; // delete exiting accel structure.

        m_sparseVolumeAccel = new lsgl::render::SparseVolumeAccel();
        return m_sparseVolumeAccel->Build(m_sparseVolume);
    }

};

BufferSparseVolumeData::BufferSparseVolumeData()
{
    m_imp = new BufferSparseVolumeData::Impl();
}

BufferSparseVolumeData::BufferSparseVolumeData(BufferSparseVolumeData* inst)
{
    m_imp = new BufferSparseVolumeData::Impl(inst);
}

BufferSparseVolumeData::~BufferSparseVolumeData()
{
    delete m_imp;
}

void BufferSparseVolumeData::Create(int w, int h, int d, int component)
{
    m_imp->Create(w, h, d, component);
}

void BufferSparseVolumeData::AddVolume(int offset_x, int offset_y, int offset_z,
               BufferVolumeData* vol)
{
    m_imp->AddVolume(offset_x, offset_y, offset_z, vol);
}

void BufferSparseVolumeData::Clear()
{
    m_imp->Clear();
}

void BufferSparseVolumeData::print()
{
    m_imp->print();
}

int BufferSparseVolumeData::Width()
{
    return m_imp->Width();
}

int BufferSparseVolumeData::Height()
{
    return m_imp->Height();
}

int BufferSparseVolumeData::Depth()
{
    return m_imp->Depth();
}

int BufferSparseVolumeData::Component()
{
    return m_imp->Component();
}

const std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::Buffers() const
{
    return m_imp->Buffers();
}

std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::Buffers()
{
    return m_imp->Buffers();
}

void BufferSparseVolumeData::Sample(float* ret, float x, float y, float z)
{
    return m_imp->Sample(ret, x, y, z);
}

bool BufferSparseVolumeData::Build()
{
    return m_imp->Build();
}

