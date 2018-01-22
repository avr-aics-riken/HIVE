/**
 * @file RenderPlugin.h
 * Renderer Interface
 */

#pragma once

#include "../Core/Ref.h"
#include "Buffer/BaseBuffer.h"
class RenderObject;
class BufferImageData;

class RenderDevice {

protected:
    bool progressCallbackFunc(int progress, int y, int height) const;
    
    class Impl;
    Impl* m_imp;
    
public:
    /// コンストラクタ
    RenderDevice();

    /// デストラクタ
    ~RenderDevice();
    
    BaseBuffer* createBuffer(const RenderObject* robj);

    int GetWidth() const;
    int GetHeight() const;


    /// バッファのクリア
    void ClearBuffers();
    
    /// レンダーオブジェクトの追加
    /// @param robj レンダーオブジェクト
    void AddRenderObject(RenderObject* robj);
    
    /// レンダーオブジェクトのクリア
    void ClearRenderObject();
    
    /// プログレスコールバックの設定
    void SetProgressCallback(bool (*func)(double));
 
    bool GetTexture(const BufferImageData* bufimg, unsigned int& id);
    
    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex);

    bool DeleteTexture(const BufferImageData* bufimg);
    
    bool CreateProgramSrc(const char* srcname, unsigned int& prg);
    
    bool ClearShaderCache(const char* srcname);
    
    /// レンダリング
    void Render();
  
    //-----------------------

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
    void BindTexture3D(unsigned int tex) const;
    void TexImage3DPointer(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const;

    void SetUniform4fv(unsigned int prg, const char* name, const float* val) const;
    void SetUniform3fv(unsigned int prg, const char* name, const float* val) const;
    void SetUniform2fv(unsigned int prg, const char* name, const float* val) const;
    void SetUniform1f(unsigned int prg, const char* name, float val) const;
    void BindProgram(unsigned int prg) const;
    void SetUniformMatrix(unsigned int prg, const char* name, const float* val) const;
    
    void BindTexture2D(unsigned int texid) const;
    void ActiveTexture(unsigned int n) const;
    
    void TexImage2D(unsigned int width, unsigned int height,
                                   unsigned int component, const unsigned char* pixeldata,
                                   bool filter, bool clampToEdgeS, bool clampToEdgeT) const;
 
    void TexImage2DFloat(unsigned int width, unsigned int height,
                                        unsigned int component, const float* pixeldata,
                                        bool filter, bool clampToEdgeS, bool clampToEdgeT) const;


    void SetUniform1i(unsigned int prg, const char* name, int val) const;
    void TexCoordRemap3D(int axis, int n, const float* values) const;
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
    void BindSolidVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const;
    void UnBindSolidVBIB(unsigned int prg) const;
    void DrawSolidArrays(int solidType, unsigned int vtxnum) const;
    void SparseTexImage3DPointer(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int depth, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const;

    void SetCamera(unsigned int prg, const float* eye, const float* lookat, const float* up, float fov) const;
    void CreateFloatBuffer(unsigned int num, float* buffer, unsigned int& buf_id) const;
    void CreateUintBuffer(unsigned int num, unsigned int* buffer, unsigned int& buf_id) const;
    void CreateVec4Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const;
    void CreateVec3Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const;
    void CreateVec2Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const;

    void BindBufferFloat(unsigned int prg, const char* attrname, unsigned int bufidx) const;
    void BindBufferUint(unsigned int prg, const char* attrname, unsigned int bufidx) const;
    void BindBufferVec4(unsigned int prg, const char* attrname, unsigned int bufidx) const;
    void BindBufferVec3(unsigned int prg, const char* attrname, unsigned int bufidx) const;
    void BindBufferVec2(unsigned int prg, const char* attrname, unsigned int bufidx) const;
    void UnBindBuffer(unsigned int prg, const char* attrname) const;

};
