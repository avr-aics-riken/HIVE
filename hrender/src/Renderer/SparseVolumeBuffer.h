/**
 * @file SparseVolumeBuffer.h
 * Volumeバッファ
 */
 
#pragma once
/*
   SparseVolumeBuffer.h
   */

#include <vector>
#include "BaseBuffer.h"
#include "../Core/Ref.h"

class SparseVolumeModel;
class BufferSparseVolumeData;

/**
 * Volumeバッファ
 */
class SparseVolumeBuffer : public BaseBuffer
{
private:
    unsigned int       m_vtx_id;
    unsigned int       m_normal_id;
    unsigned int       m_mat_id;
    unsigned int       m_tex_id;
    unsigned int       m_index_id;
    unsigned int       m_vertex_num;
    unsigned int       m_index_num;
    float              m_boxsize[3];
    float              m_voldim[3];
    double             m_volMinVal[3];
    double             m_volMaxVal[3];
    unsigned int       m_sgl_voltex;
    //std::vector<float> m_volHist[3];

    RefPtr<const SparseVolumeModel> m_model;

    bool MakeBox(float width, float height, float depth);
    bool CreateSparseTexture3D(const BufferSparseVolumeData* volume, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR);
public:
    SparseVolumeBuffer(RENDER_MODE mode);
    ~SparseVolumeBuffer();

    bool Create(const SparseVolumeModel* model);
    void Render() const;
    void Update();
};

