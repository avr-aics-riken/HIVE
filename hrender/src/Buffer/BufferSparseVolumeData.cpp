/**
 * @file BufferSparseVolumeData.cpp
 * BufferSparseVolumeDataクラス
 */
#include "BufferSparseVolumeData.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

// Use SparseVolume feature from SURFACE to provide Sample() function
#include "render_bvh_tree.h"

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
	lsgl::render::BVHTree m_tree;

	// Grabbed from SURFACE/render/render_accel_volume.cc
	bool SampleSparseVolume(
		double value[4],
		const double position[3]) {

	  StackVector<lsgl::render::BVHNodeLocator, 32> locaters;
	  bool hit = m_tree.Locate(locaters, position);

	  value[0] = 0.0f;
	  value[1] = 0.0f;
	  value[2] = 0.0f;
	  value[3] = 1.0f; 

	  if (hit) {
		// @note { We don't allow overlapping of volume block. }
		lsgl::render::BVHNodeLocator locator = locaters[0];

		// fetch texel color
		int blockID = locator.nodeID;
		int components = m_components;

	   	const VolumeBlock &block = m_volumeBlocks[blockID];

		// find local offset.
		double x = position[0] - block.offset[0];
		double y = position[1] - block.offset[1];
		double z = position[2] - block.offset[2];
		int ix = (std::max)(
			(std::min)(block.offset[0] + block.volume->Width() - 1, (int)x), 0);
		int iy = (std::max)(
			(std::min)(block.offset[1] + block.volume->Height() - 1, (int)y), 1);
		int iz = (std::max)(
			(std::min)(block.offset[2] + block.volume->Depth() - 1, (int)z), 2);

		 size_t idx =
			  iz * block.volume->Width()  * block.volume->Height() + iy * block.volume->Depth() + ix;
		for (int c = 0; c < components; c++) {
			value[c] = (block.volume->Buffer()->GetBuffer())[components * idx + c];
		}

	  }

	  return hit;
	}
		

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

        //// Also add to SparseVolume
        //{
        //    lsgl::render::VolumeBlock block;
        //    block.offset[0] = offset_x;
        //    block.offset[1] = offset_y;
        //    block.offset[2] = offset_z;

        //    block.extent[0] = vol->Width();
        //    block.extent[1] = vol->Height();
        //    block.extent[2] = vol->Depth();

        //    assert(block.offset[0] + block.extent[0] <= Width());
        //    assert(block.offset[1] + block.extent[1] <= Height());
        //    assert(block.offset[2] + block.extent[2] <= Depth());

        //    block.id = m_volumeBlocks.size();
        //    block.data = reinterpret_cast<unsigned char*>(vol->Buffer()->GetBuffer()); // @fixme { Take a pointer reference. Is this OK? }

        //    m_sparseVolume.blocks.push_back(block);
        //}


    }
    
    /// メンバクリア
    void Clear()
    {
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
		SampleSparseVolume(value, position);
#endif
        for (int c = 0; c < m_components; c++) {
            ret[c] = value[c];
        }

    }

    /// 疎ボリュームデータをビルド
    bool Build() {

		if (m_volumeBlocks.empty()) {
			return false;
		}

#ifndef _WIN32

		for (size_t i = 0; i < m_volumeBlocks.size(); i++) {

			lsgl::render::BVHData data;

			data.bmin[0] = m_volumeBlocks[i].offset[0];
			data.bmin[1] = m_volumeBlocks[i].offset[1];
			data.bmin[2] = m_volumeBlocks[i].offset[2];

			data.bmax[0] = data.bmin[0] + m_volumeBlocks[i].volume->Width();
			data.bmax[1] = data.bmin[1] + m_volumeBlocks[i].volume->Height();
			data.bmax[2] = data.bmin[2] + m_volumeBlocks[i].volume->Depth();

			data.nodeID = i;

			m_tree.AddNode(data);
		}

		m_tree.BuildTree();
		return true;
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

