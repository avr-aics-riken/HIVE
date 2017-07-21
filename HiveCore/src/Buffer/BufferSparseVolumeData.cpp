/**
 * @file BufferSparseVolumeData.cpp
 * BufferSparseVolumeDataクラス
 */
#include "BufferSparseVolumeData.h"
#include "Buffer.h"
#include "BufferVolumeData.h"

// Use SparseVolume feature from SURFACE to provide Sample() function
#include "render_bvh_tree.h"

#include <limits>
#include <cstring>

/**
 * BufferSparseVolumeDataクラス
 */
class BufferSparseVolumeData::Impl
{
private:
    int m_dim[3];       ///< Extent dim.
    std::vector<VolumeBlock> m_volumeBlocks;

	// For `Sample()` method.
	lsgl::render::BVHTree m_tree;
	bool m_isBuilt;

	// Grabbed from SURFACE/render/render_accel_volume.cc
	bool SampleSparseVolume(
		double value[16],
		const double position[3]) {

	  StackVector<lsgl::render::BVHNodeLocator, 32> locaters;
	  bool hit = m_tree.Locate(locaters, position);

      memset(value, 0, sizeof(double) * 16);

	  if (hit) {
		// @note { We don't allow overlapping of volume blocks. }
		lsgl::render::BVHNodeLocator locator = locaters[0];

		// fetch texel color
		int blockID = locator.nodeID;

	   	const VolumeBlock &block = m_volumeBlocks[blockID];

		// find local offset.
		double x = position[0] - block.offset[0];
		double y = position[1] - block.offset[1];
		double z = position[2] - block.offset[2];

        if (block.isRaw) {
		    int components = block.components;
            int numChannels = (components > 16) ? 16 : components; // Up to 16 channels.

            // map [0, extent] to cell size: [0, size].
            double scaleX = block.size[0] / (double)block.extent[0];
            double scaleY = block.size[1] / (double)block.extent[1];
            double scaleZ = block.size[2] / (double)block.extent[2];

            x = x * scaleX;
            y = y * scaleX;
            z = z * scaleX;

            int ix = (std::max)(
                (std::min)(block.size[0] - 1, (int)x), 0);
            int iy = (std::max)(
                (std::min)(block.size[1] - 1, (int)y), 0);
            int iz = (std::max)(
                (std::min)(block.size[2] - 1, (int)z), 0);

            size_t idx =
                  iz * block.size[1] * block.size[0] + iy * block.size[0] + ix;
            if (block.format == BufferSparseVolumeData::FORMAT_FLOAT) {
                const float* ptr = reinterpret_cast<const float*>(block.rawData);
                for (int c = 0; c < numChannels; c++) {
                    value[c] = ptr[components * idx + c];
                }
            } else {
                const double* ptr = reinterpret_cast<const double*>(block.rawData);
                for (int c = 0; c < numChannels; c++) {
                    value[c] = ptr[components * idx + c];
                }
            }

        } else {

		    int components = block.volume->Component();
            int numChannels = (components > 16) ? 16 : components; // Up to 16 channels.

            // map [0, extent] to cell size: [0, size].
            double scaleX = block.volume->Width() / (double)block.extent[0];
            double scaleY = block.volume->Height() / (double)block.extent[1];
            double scaleZ = block.volume->Depth() / (double)block.extent[2];

            x = x * scaleX;
            y = y * scaleX;
            z = z * scaleX;

            int ix = (std::max)(
                (std::min)(block.volume->Width() - 1, (int)x), 0);
            int iy = (std::max)(
                (std::min)(block.volume->Height() - 1, (int)y), 0);
            int iz = (std::max)(
                (std::min)(block.volume->Depth() - 1, (int)z), 0);

             size_t idx =
                  iz * block.volume->Height()  * block.volume->Width() + iy * block.volume->Width() + ix;
            for (int c = 0; c < numChannels; c++) {
                value[c] = (block.volume->Buffer()->GetBuffer())[components * idx + c];
            }
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
		m_isBuilt = false;

    }
    
    /// コンストラクタ
    /// @param inst 疎ボリュームデータ
    Impl(BufferSparseVolumeData* inst)
    {
		m_dim[0] = inst->Width();
		m_dim[1] = inst->Height();
		m_dim[2] = inst->Depth();

		m_volumeBlocks = inst->VolumeBlocks();

		// @todo { m_tree }
		m_isBuilt = inst->IsBuilt();
    }
    
    /// デストラクタ
    ~Impl()
    {
    }

    /// BufferSparseVolumeDataの作成
    void Create()
    {
		m_dim[0] = 0;
		m_dim[1] = 0;
		m_dim[2] = 0;
    }
    
    /**
     * BufferSparseVolumeDataへのBufferVolumeDataの追加
     * @param level    LoD level
     * @param offset_x SparseBlockオフセットX(World coordinate)
     * @param offset_y SparseBlockオフセットY(World coordiante)
     * @param offset_z SparseBlockオフセットZ(World coordiante)
     * @param extent_x SparseBlock Extent X(World coordinate)
     * @param extent_y SparseBlock Extent Y(World coordinate)
     * @param extent_z SparseBlock Extent Z(World coordinate)
     * @param vol      対象のBufferVolumeData
     */
    void AddVolume(int level, int offset_x, int offset_y, int offset_z,
                   int extent_x, int extent_y, int extent_z, BufferVolumeData* vol)
    {
		{
            VolumeBlock block;
            block.isRaw = false;
            block.rawData = NULL;
            block.level = level;
            block.offset[0] = offset_x;
            block.offset[1] = offset_y;
            block.offset[2] = offset_z;
            block.extent[0] = extent_x;
            block.extent[1] = extent_y;
            block.extent[2] = extent_z;

            m_dim[0] = std::max(block.offset[0] + block.extent[0], m_dim[0]);
            m_dim[1] = std::max(block.offset[1] + block.extent[1], m_dim[1]);
            m_dim[2] = std::max(block.offset[2] + block.extent[2], m_dim[2]);

			block.volume = vol; // pointer reference

			m_volumeBlocks.push_back(block);
		}
    }

    void AddRAWVolume(int level, int offset_x, int offset_y, int offset_z,
                   int extent_x, int extent_y, int extent_z, int size_x, int size_y, int size_z, int components, VolumeBlockFormat format, const void *data)
    {
		{
            VolumeBlock block;
            block.isRaw = true;
            block.volume = NULL;
            block.level = level;
            block.offset[0] = offset_x;
            block.offset[1] = offset_y;
            block.offset[2] = offset_z;
            block.extent[0] = extent_x;
            block.extent[1] = extent_y;
            block.extent[2] = extent_z;
            block.size[0] = size_x;
            block.size[1] = size_y;
            block.size[2] = size_z;
            block.components = components;
            block.format = format;

            m_dim[0] = std::max(block.offset[0] + block.extent[0], m_dim[0]);
            m_dim[1] = std::max(block.offset[1] + block.extent[1], m_dim[1]);
            m_dim[2] = std::max(block.offset[2] + block.extent[2], m_dim[2]);

			block.rawData = reinterpret_cast<const unsigned char*>(data); // pointer reference

			m_volumeBlocks.push_back(block);
		}
    }
    
    /// メンバクリア
    void Clear()
    {
		m_volumeBlocks.clear();
		m_dim[0] = -1;
		m_dim[1] = -1;
		m_dim[2] = -1;
		m_tree.Clear();
		m_isBuilt = false;
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
    
	const std::vector<VolumeBlock>& VolumeBlocks() const {
		return m_volumeBlocks;
	}

	std::vector<VolumeBlock>& VolumeBlocks() {
		return m_volumeBlocks;
	}
    
    /**
     * サンプルする
     * @param ret サンプル結果(up to 16)
     * @param x x(normalized)
     * @param y y(normalized)
     * @param z z(normalized)
     */
    void Sample(float* ret, float x, float y, float z) {

        // Up to 16 components at this time.

        double value[16];
        double position[3];

		position[0] = x * Width();
		position[1] = y * Height();
		position[2] = z * Depth();

		SampleSparseVolume(value, position);

        for (int c = 0; c < 16; c++) {
            ret[c] = value[c];
        }

    }

    /// 疎ボリュームデータをビルド
    bool Build() {

		if (m_volumeBlocks.empty()) {
			return false;
		}

		if (m_isBuilt) {
			return true;
		}

		double minval = std::numeric_limits<double>::max();
		double maxval = -std::numeric_limits<double>::max();

		for (size_t i = 0; i < m_volumeBlocks.size(); i++) {
            if (m_volumeBlocks[i].isRaw) {
      
                const BufferVolumeData* vb = m_volumeBlocks[i].volume;

                size_t n = m_volumeBlocks[i].size[0] * m_volumeBlocks[i].size[1] * m_volumeBlocks[i].size[2] * m_volumeBlocks[i].components;
                for (size_t j = 0; j < n; j++) {
                    double val = m_volumeBlocks[i].rawData[j];
                    minval = (std::min)(minval, val);
                    maxval = (std::max)(maxval, val);
                }
            } else {
                const BufferVolumeData* vb = m_volumeBlocks[i].volume;

                for (size_t j = 0; j < vb->Buffer()->GetNum(); j++) {
                    double val = vb->Buffer()->GetBuffer()[j];
                    minval = (std::min)(minval, val);
                    maxval = (std::max)(maxval, val);
                }
            }
		}
		printf("[DBG] minval = %f, maxval = %f\n", minval, maxval);

		for (size_t i = 0; i < m_volumeBlocks.size(); i++) {

			lsgl::render::BVHData data;

			data.bmin[0] = m_volumeBlocks[i].offset[0];
			data.bmin[1] = m_volumeBlocks[i].offset[1];
			data.bmin[2] = m_volumeBlocks[i].offset[2];

            if (m_volumeBlocks[i].isRaw) {

                data.bmax[0] = data.bmin[0] + m_volumeBlocks[i].extent[0];
                data.bmax[1] = data.bmin[1] + m_volumeBlocks[i].extent[1];
                data.bmax[2] = data.bmin[2] + m_volumeBlocks[i].extent[2];

            } else {

                data.bmax[0] = data.bmin[0] + m_volumeBlocks[i].volume->Width();
                data.bmax[1] = data.bmin[1] + m_volumeBlocks[i].volume->Height();
                data.bmax[2] = data.bmin[2] + m_volumeBlocks[i].volume->Depth();

            }

			data.nodeID = i;

			m_tree.AddNode(data);
		}

		m_tree.BuildTree();

		double bmin[3], bmax[3];
		m_tree.BoundingBox(bmin, bmax);
		printf("[BufferSparseVolumeData] bmin = (%f, %f, %f)\n", bmin[0], bmin[1], bmin[2]);
		printf("[BufferSparseVolumeData] bmax = (%f, %f, %f)\n", bmax[0], bmax[1], bmax[2]);

		m_isBuilt = true;
		return true;

    }

	bool IsBuilt() const {
		return m_isBuilt;
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


BufferSparseVolumeData* BufferSparseVolumeData::CreateInstance()
{
    return new BufferSparseVolumeData();
}


void BufferSparseVolumeData::Create()
{
    m_imp->Create();
}

/**
 * BufferSparseVolumeDataへのBufferVolumeDataの追加
 * @param level    LoD level
 * @param offset_x SparseBlockオフセットX
 * @param offset_y SparseBlockオフセットY
 * @param offset_z SparseBlockオフセットZ
 * @param extent_x Extent in X 
 * @param extent_y Extent in Y
 * @param extent_z Extent in Z
 * @param vol      対象のBufferVolumeData
 */
void BufferSparseVolumeData::AddVolumeBlock(int level, int offset_x, int offset_y, int offset_z,
               int extent_x, int extent_y, int extent_z, BufferVolumeData* vol)
{
    m_imp->AddVolume(level, offset_x, offset_y, offset_z, extent_x, extent_y, extent_z, vol);
}

/**
 * BufferSparseVolumeDataへの Raw VolumeBlock の追加
 * @param level    LoD level
 * @param offset_x SparseBlockオフセットX
 * @param offset_y SparseBlockオフセットY
 * @param offset_z SparseBlockオフセットZ
 * @param extent_x Extent in X 
 * @param extent_y Extent in Y
 * @param extent_z Extent in Z
 * @param size_x   Cell size in X 
 * @param size_y   Cell size in Y
 * @param size_z   Cell size in Z
 * @param component # of components in voxel
 * @param format    Voxel data format
 * @param data  Pointer to raw volume data
 */
void BufferSparseVolumeData::AddRAWVolumeBlock(int level, int offset_x, int offset_y, int offset_z,
               int extent_x, int extent_y, int extent_z, int size_x, int size_y, int size_z, int components, VolumeBlockFormat format, const void* data)
{
    m_imp->AddRAWVolume(level, offset_x, offset_y, offset_z, extent_x, extent_y, extent_z, size_x, size_y, size_z, components, format, data);
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
int BufferSparseVolumeData::Width() const
{
    return m_imp->Width();
}

/// Height取得
int BufferSparseVolumeData::Height() const
{
    return m_imp->Height();
}

/// Depth取得
int BufferSparseVolumeData::Depth() const
{
    return m_imp->Depth();
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

bool BufferSparseVolumeData::IsBuilt() const
{
    return m_imp->IsBuilt();
}
