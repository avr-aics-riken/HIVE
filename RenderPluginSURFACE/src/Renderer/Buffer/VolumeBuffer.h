/**
 * @file VolumeBuffer.h
 * Volumeバッファ
 */
 
#pragma once
/*
   VolumeBuffer.h
   */

#include <vector>
#include "BaseBuffer.h"
#include "../Core/Ref.h"

class VolumeModel;
class BufferVolumeData;

/**
 * Volumeバッファ
 */
class VolumeBuffer : public BaseBuffer
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
    unsigned int       m_volcomp;
    double             m_volMinVal[3];
    double             m_volMaxVal[3];
    unsigned int       m_sgl_voltex;
    std::vector<float> m_volHist[3];

    RefPtr<const VolumeModel> m_model;

    bool MakeBox(float width, float height, float depth);
    bool CreateTexture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR);
public:
    VolumeBuffer(RenderDevice* render);
    ~VolumeBuffer();

    bool Create(const VolumeModel* model);
    void Render() const;
    void Update();
    void Clear();
};

