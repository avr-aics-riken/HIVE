/**
 * @file RenderPlugin.h
 * Renderer Interface
 */

#pragma once

#include "../Core/Ref.h"
#include "BaseBuffer.h"
class RenderObject;
class BufferImageData;

class RenderPlugin {

protected:
    virtual bool progressCallbackFunc(int progress, int y, int height) const = 0;
    
    /// コンストラクタ
    RenderPlugin(){};
    
public:
    /// デストラクタ
    virtual ~RenderPlugin(){};
    
    BaseBuffer* createBuffer(const RenderObject* robj);

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;


    /// バッファのクリア
    virtual void ClearBuffers() = 0;
    
    /// レンダーオブジェクトの追加
    /// @param robj レンダーオブジェクト
    virtual void AddRenderObject(RenderObject* robj) = 0;
    
    /// レンダーオブジェクトのクリア
    virtual void ClearRenderObject() = 0;
    
    /// プログレスコールバックの設定
    virtual void SetProgressCallback(bool (*func)(double)) = 0;
 
    virtual bool GetTexture(const BufferImageData* bufimg, unsigned int& id) = 0;
    
    virtual bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex) = 0;

    virtual bool DeleteTexture(const BufferImageData* bufimg) = 0;
    
    virtual bool CreateProgramSrc(const char* srcname, unsigned int& prg) = 0;
    
    virtual bool ClearShaderCache(const char* srcname) = 0;
    
    /// レンダリング
    virtual void Render() = 0;
  
    //-----------------------

    virtual void ReleaseBuffer(unsigned int bufferId) const = 0;
    virtual void CreateVBIB(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                    float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                    unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                    unsigned int& tex_id, unsigned int& index_id) const = 0;
    virtual void BindVBIB(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
                unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) const = 0;
    virtual void UnBindVBIB(unsigned int prg) const = 0;
    virtual void DrawElements(unsigned int indexnum) const = 0;
    virtual void DrawArrays(unsigned int vtxnum) const = 0;
    virtual void GenTextures(int n, unsigned int* tex) const = 0;
    virtual void BindTexture3D(unsigned int tex) const = 0;
    virtual void TexImage3DPointer(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const = 0;

    virtual void SetUniform4fv(unsigned int prg, const char* name, const float* val) const = 0;
    virtual void SetUniform3fv(unsigned int prg, const char* name, const float* val) const = 0;
    virtual void SetUniform2fv(unsigned int prg, const char* name, const float* val) const = 0;
    virtual void SetUniform1f(unsigned int prg, const char* name, float val) const = 0;
    virtual void BindProgram(unsigned int prg) const = 0;
    virtual void SetUniformMatrix(unsigned int prg, const char* name, const float* val) const = 0;
    
    virtual void BindTexture2D(unsigned int texid) const = 0;
    virtual void ActiveTexture(unsigned int n) const = 0;
    
    virtual void TexImage2D(unsigned int width, unsigned int height,
                                   unsigned int component, const unsigned char* pixeldata,
                                   bool filter, bool clampToEdgeS, bool clampToEdgeT) const = 0;
 
    virtual void TexImage2DFloat(unsigned int width, unsigned int height,
                                        unsigned int component, const float* pixeldata,
                                        bool filter, bool clampToEdgeS, bool clampToEdgeT) const = 0;


    virtual void SetUniform1i(unsigned int prg, const char* name, int val) const = 0;
    virtual void TexCoordRemap3D(int axis, int n, const float* values) const = 0;
    virtual void BindLineVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) const = 0;
    virtual void UnBindLineVBIB(unsigned int prg) const = 0;
    virtual void DrawLineElements(unsigned int indexnum) const = 0;
    virtual void DrawLineArrays(unsigned int vtxnum) const = 0;
    virtual void CreateVBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                    unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) const = 0;
    virtual void CreateVBIBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                unsigned int indexnum, unsigned int* indexbuffer,
                                unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id) const = 0;
    virtual void LineWidth(float w) const = 0;
    virtual void BindPointVB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) const = 0;
    virtual void UnBindPointVB(unsigned int prg) const = 0;
    virtual void DrawPointArrays(unsigned int vtxnum) const = 0;
    virtual void BindTetraVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const = 0;
    virtual void UnBindTetraVBIB(unsigned int prg) const = 0;
    virtual void DrawTetraArrays(unsigned int vtxnum) const = 0;
    virtual void BindSolidVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const = 0;
    virtual void UnBindSolidVBIB(unsigned int prg) const = 0;
    virtual void DrawSolidArrays(int solidType, unsigned int vtxnum) const = 0;
    virtual void SparseTexImage3DPointer(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int depth, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const = 0;

    virtual void SetCamera(unsigned int prg, const float* eye, const float* lookat, const float* up, float fov) const = 0;
    virtual void CreateFloatBuffer(unsigned int num, float* buffer, unsigned int& buf_id) const = 0;
    virtual void CreateUintBuffer(unsigned int num, unsigned int* buffer, unsigned int& buf_id) const = 0;
    virtual void CreateVec4Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const = 0;
    virtual void CreateVec3Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const = 0;
    virtual void CreateVec2Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const = 0;

    virtual void BindBufferFloat(unsigned int prg, const char* attrname, unsigned int bufidx) const = 0;
    virtual void BindBufferUint(unsigned int prg, const char* attrname, unsigned int bufidx) const = 0;
    virtual void BindBufferVec4(unsigned int prg, const char* attrname, unsigned int bufidx) const = 0;
    virtual void BindBufferVec3(unsigned int prg, const char* attrname, unsigned int bufidx) const = 0;
    virtual void BindBufferVec2(unsigned int prg, const char* attrname, unsigned int bufidx) const = 0;
    virtual void UnBindBuffer(unsigned int prg, const char* attrname) const = 0;

};
