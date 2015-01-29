#pragma once
/*
   VolumeBuffer.h
   */

#include <vector>
#include "BaseBuffer.h"

class VolumeModel;

class VolumeBuffer : public BaseBuffer
{
private:
    unsigned int m_vtx_id;
    unsigned int m_normal_id;
    unsigned int m_mat_id;
    unsigned int m_tex_id;
    unsigned int m_index_id;
    unsigned int m_vertex_num;
    unsigned int m_index_num;

    float  m_boxsize[3];
    float  m_voldim[3];
    std::vector<float> m_volHist[3];
    double m_volMinVal[3];
    double m_volMaxVal[3];

    //3DTexture
    unsigned int m_sgl_voltex;

    bool MakeBox(float width, float height, float depth);
bool CreateTexture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata);
public:
    VolumeBuffer(RENDER_MODE mode);
    ~VolumeBuffer();

    bool Create(const VolumeModel* model);
    void Render() const;
};

