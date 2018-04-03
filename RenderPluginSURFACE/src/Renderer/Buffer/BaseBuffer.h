/**
 * @file BaseBuffer.h
 * ベースバッファ
 */
#ifndef __BASEBUFFER_H__
#define __BASEBUFFER_H__

/*
    BaseBuffer.h
 */

#include "../Core/Ref.h"

class Camera;
class RenderObject;
class BufferImageData;
class RenderDevice;

#include <map>
#include <string>

/**
 * ベースバッファ
 */
class BaseBuffer : public RefCount
{
protected:
    BaseBuffer(RenderDevice* render);
    virtual ~BaseBuffer();
    
    RenderDevice* m_render;
    unsigned int m_prog;
    
    std::map<std::string, unsigned int> m_extraIdx;
    
public:
    virtual void Render() const = 0;
    virtual void Update() = 0;
    
    void BindProgram() const;
    void Uniform2fv(const char* name, const float*) const;
    void Uniform4fv(const char* name, const float*) const;
    void SetCamera(const Camera* camera) const;
    void UnbindProgram() const;
    void ReleaseBuffer(unsigned int bufferId) const;
    void CreateVBIB(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                    float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                    unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                    unsigned int& tex_id, unsigned int& index_id) const;
    void BindVBIB(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
                  unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) const;
    void UnBindVBIB(unsigned int prg) const;
    void DrawElements(unsigned int indexnum) const;
    void DrawArrays(unsigned int vtxnum) const;
    void GenTextures(int n, unsigned int* tex) const;

    void SetUniform3fv(unsigned int prg, const char* name, const float* val) const;
    void SetUniform1i(unsigned int prg, const char* name, int val) const;

    
    void BindLineVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) const;
    void UnBindLineVBIB(unsigned int prg) const;
    void DrawLineElements(unsigned int indexnum) const;
    void DrawLineArrays(unsigned int vtxnum) const;
    void CreateVBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                    unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) const;
    void CreateVBIBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                  unsigned int indexnum, unsigned int* indexbuffer,
                      unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id) const;
    
    void LineWidth(float w) const;
    
    
    void BindPointVB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) const;
    void UnBindPointVB(unsigned int prg) const;
    void DrawPointArrays(unsigned int vtxnum) const;

    
    void BindTetraVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const;
    void UnBindTetraVBIB(unsigned int prg) const;
    void DrawTetraArrays(unsigned int vtxnum) const;

    
    // for SURFACE
    void BindTexture3D(unsigned int tex) const;
    void TexImage3DPointer(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const;
    void TexCoordRemap3D(int axis, int n, const float* values) const;

    void BindSolidVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const;
    void UnBindSolidVBIB(unsigned int prg) const;

    void DrawSolidArrays(int solidType, unsigned int vtxnum) const;
    void SparseTexImage3DPointer(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset,
        unsigned int width, unsigned int height, unsigned int depth,
        unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth,
        unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const;

    //-------------------------------------------------------------------

    bool loadShaderSrc(const char* srcname);
    unsigned int getProgram() const;
    void bindUniforms(const RenderObject* obj) const;
    void createExtraBuffers(const RenderObject* obj);
    void bindExtraBuffers(const RenderObject* obj) const;
    void unbindExtraBuffers(const RenderObject* obj) const;
    unsigned int getTextureId(const BufferImageData* buf) const;
    bool cacheTexture(const BufferImageData* buf, bool filter, bool clampToEdgeS, bool clampToEdgeT);
    void cacheTextures(const RenderObject* model);
};

#endif // __BASEBUFFER_H__

