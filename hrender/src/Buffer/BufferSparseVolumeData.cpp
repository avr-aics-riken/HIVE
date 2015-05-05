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
    int m_components;
    std::vector<VolumeBlock> m_volumeBlocks;

	// For `Sample()` method.
    lsgl::render::SparseVolumeAccel m_sparseVolumeAccel;
    lsgl::render::SparseVolume m_sparseVolume;

public:
    
    /// コンストラクタ
    Impl()
    {
		m_dim[0] = -1;
		m_dim[1] = -1;
		m_dim[2] = -1;
    }
    
    /// コンストラクタ
    /// @param inst 疎ボリュームデータ
    Impl(BufferSparseVolumeData* inst)
    {
		m_dim[0] = inst->Width();
		m_dim[1] = inst->Height();
		m_dim[2] = inst->Depth();
		m_components = inst->Component();

		m_volumeBlocks = inst->VolumeBlocks();

		// @todo { m_sparseVolumeAccel, _sparseVolume }
    }
    
    /// デストラクタ
    ~Impl()
    {
    }

    /// BufferSparseVolumeDataの作成
    void Create(int w, int h, int d, int component)
    {
		m_dim[0] = w;
		m_dim[1] = h;
		m_dim[2] = d;
		m_components = component;

		//m_sparseVolume = new lsgl::render::SparseVolume();
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
		{
            VolumeBlock block;
            block.offset[0] = offset_x;
            block.offset[1] = offset_y;
            block.offset[2] = offset_z;
			block.volume = vol; // pointer reference

			m_volumeBlocks.push_back(block);
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
            assert(block.offset[1] + block.extent[1] <= Height());
            assert(block.offset[2] + block.extent[2] <= Depth());

            block.id = m_volumeBlocks.size();
            block.data = reinterpret_cast<unsigned char*>(vol->Buffer()->GetBuffer()); // @fixme { Take a pointer reference. Is this OK? }

            m_sparseVolume.blocks.push_back(block);
        }


    }
    
    /// メンバクリア
    void Clear()
    {
        m_sparseVolume.blocks.clear();

		m_volumeBlocks.clear();
		m_dim[0] = -1;
		m_dim[1] = -1;
		m_dim[2] = -1;
		m_components = -1;
    }
    
    /// デバッグ用
    void print()
    {
    }
    
    /// Width取得
    int Width()
    {
        return m_dim[0];
    }
    
    /// Height取得
    int Height()
    {
        return m_dim[1];
    }
    
    /// Depth取得
    int Depth()
    {
        return m_dim[2];
    }
    
    /// Component数取得
    int Component()
    {
		return m_components;
    }

	const std::vector<VolumeBlock>& VolumeBlocks() const {
		return m_volumeBlocks;
	}

	std::vector<VolumeBlock>& VolumeBlocks() {
		return m_volumeBlocks;
	}
    
    /**
     * サンプルする
     * @param ret サンプル結果
     * @param x x
     * @param y y
     * @param z z
     */
    void Sample(float* ret, float x, float y, float z) {

        // Up to 4 components at this time.
        assert(m_components <= 4);

        double value[4];
        double position[3];

        position[0] = x * Width();
        position[1] = y * Height();
        position[2] = z * Depth();
#ifndef _WIN32
        m_sparseVolumeAccel.Sample(value, position);
#endif
        for (int c = 0; c < m_components; c++) {
            ret[c] = value[c];
        }

    }

    /// 疎ボリュームデータをビルド
    bool Build() {
        //delete m_sparseVolumeAccel; // delete exiting accel structure.

		//m_sparseVolumeAccel = new lsgl::render::SparseVolumeAccel();
		if (m_sparseVolume.blocks.empty()) {
			return false;
		}
#ifndef _WIN32
        return m_sparseVolumeAccel.Build(&m_sparseVolume);
#else
		return false;
#endif
    }

};

/// コンストラクタ
BufferSparseVolumeData::BufferSparseVolumeData() : BufferData(TYPE_SPARSEVOLUME)
{
    m_imp = new BufferSparseVolumeData::Impl();
}

/// コンストラクタ
/// @param inst 疎ボリュームデータ
BufferSparseVolumeData::BufferSparseVolumeData(BufferSparseVolumeData* inst) : BufferData(TYPE_SPARSEVOLUME)
{
    m_imp = new BufferSparseVolumeData::Impl(inst);
}

/// デストラクタ
BufferSparseVolumeData::~BufferSparseVolumeData()
{
    delete m_imp;
}

void BufferSparseVolumeData::Create(int w, int h, int d, int component)
{
    m_imp->Create(w, h, d, component);
}

/**
 * BufferSparseVolumeDataへのBufferVolumeDataの追加
 * @param offset_x SparseBlockオフセットX
 * @param offset_y SparseBlockオフセットY
 * @param offset_z SparseBlockオフセットZ
 * @param vol      対象のBufferVolumeData
 */
void BufferSparseVolumeData::AddVolume(int offset_x, int offset_y, int offset_z,
               BufferVolumeData* vol)
{
    m_imp->AddVolume(offset_x, offset_y, offset_z, vol);
}

/// メンバクリア
void BufferSparseVolumeData::Clear()
{
    m_imp->Clear();
}

/// デバッグ用
void BufferSparseVolumeData::print()
{
    m_imp->print();
}

/// Width取得
const int BufferSparseVolumeData::Width() const
{
    return m_imp->Width();
}

/// Height取得
const int BufferSparseVolumeData::Height() const
{
    return m_imp->Height();
}

/// Depth取得
const int BufferSparseVolumeData::Depth() const
{
    return m_imp->Depth();
}

/// Component数取得
const int BufferSparseVolumeData::Component() const
{
    return m_imp->Component();
}

/// バッファの参照を返す
const std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::VolumeBlocks() const
{
    return m_imp->VolumeBlocks();
}

/// バッファの参照を返す
std::vector<BufferSparseVolumeData::VolumeBlock>& BufferSparseVolumeData::VolumeBlocks()
{
    return m_imp->VolumeBlocks();
}

/**
 * サンプルする
 * @param ret サンプル結果
 * @param x x
 * @param y y
 * @param z z
 */
void BufferSparseVolumeData::Sample(float* ret, float x, float y, float z)
{
    return m_imp->Sample(ret, x, y, z);
}

/// 疎ボリュームデータをビルド
bool BufferSparseVolumeData::Build()
{
    return m_imp->Build();
}

