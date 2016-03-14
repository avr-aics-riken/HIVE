/**
 * @file Commands.h
 * SGLコマンドAPI
 */
//
//  Commands.h
//  AnimTool
//
//  Created by kioku on 13/02/08.
//

#ifndef __AnimTool__Commands__
#define __AnimTool__Commands__

void CreateBuffer_SGL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer, int colorbit, unsigned int& depthRenderbuffer, int depthbit);
void ReleaseBuffer_SGL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer);
void Clear_SGL(float red, float green, float blue, float alpha);
void GetColorBuffer_SGL(int w, int h, unsigned char* imgbuf, int colorbit);
void GetDepthBuffer_SGL(int w, int h, float* depthbuf);
void CreateFloatBuffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id);
void CreateUintBuffer_SGL(unsigned int num, unsigned int* buffer, unsigned int& buf_id);
void CreateVec4Buffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id);
void CreateVec3Buffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id);
void CreateVec2Buffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id);
void CreateVBIB_SGL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
					unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id, unsigned int& tex_id, unsigned int& index_id);
void ReleaseBufferVBIB_SGL(unsigned int buffer_id);
void CreateVBRM_SGL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
					unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id);
void CreateVBIBRM_SGL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                      unsigned int indexnum, unsigned int* indexbuffer,
                      unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id);

void BindProgram_SGL(unsigned int prg);
void SetUniform1i_SGL(unsigned int prg, const char* name, int val);
void SetUniform1f_SGL(unsigned int prg, const char* name, const float val);
void SetUniform2fv_SGL(unsigned int prg, const char* name, const float* val);
void SetUniform3fv_SGL(unsigned int prg, const char* name, const float* val);
void SetUniform4fv_SGL(unsigned int prg, const char* name, const float* val);
void SetUniformFloatArray_SGL(unsigned int prg, const char* name, const float* val, int num);
void SetUniformMatrix_SGL(unsigned int prg, const char* name, const float* val);
void SetCamera_SGL(unsigned int prg, const float* eye, const float* lookat, const float* up, float fov);
void SetStereoEnvCamera_SGL(unsigned int prg, const float* eye, const float* lookat, const float* up, float zeroParallax, float eyeSeparation);
void DrawElements_SGL(unsigned int indexnum);
void DrawLineElements_SGL(unsigned int indexnum);
void DrawArrays_SGL(unsigned int vtxnum);
void DrawPointArrays_SGL(unsigned int vertexnum);
void DrawLineArrays_SGL(unsigned int vertexnum);
void DrawTetraArrays_SGL(unsigned int vertexnum);
void DrawSolidArrays_SGL(int solidType, unsigned int vertexnum);
void SampleCoverage_SGL(float a, bool invert);
void PixelStep_SGL(int n);
void SetShaderCompiler_SGL(const char* path, const char* opt);
bool CreateProgramSrc_SGL(const char* srcname, unsigned int& prg);
bool CreateProgramBinary_SGL(const char* soname, unsigned int& prg);
bool DeleteProgram_SGL(unsigned int prg);
void BindBufferFloat_SGL(unsigned int prg, const char* attrname, unsigned int bufidx);
void BindBufferUint_SGL(unsigned int prg, const char* attrname, unsigned int bufidx);
void BindBufferVec4_SGL(unsigned int prg, const char* attrname, unsigned int bufidx);
void BindBufferVec3_SGL(unsigned int prg, const char* attrname, unsigned int bufidx);
void BindBufferVec2_SGL(unsigned int prg, const char* attrname, unsigned int bufidx);
void BindVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int normalidx, unsigned int matidx, unsigned int texidx, unsigned int indexidx);
void BindPointVB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material);
void BindLineVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx);
void BindTetraVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx);
void BindSolidVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx);
void LineWidth_SGL(float w);

void GenTextures_SGL(int n, unsigned int* tex);
void DeleteTextures_SGL(int n, unsigned int* tex);
void BindTexture3D_SGL(unsigned int tex);
void BindTexture2D_SGL(unsigned int tex);
void ActiveTexture_SGL(unsigned int n);
void TexImage2D_SGL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata, bool filter, bool clampToEdgeS, bool clampToEdgeT);
void TexImage2DFloat_SGL(unsigned int width, unsigned int height, unsigned int component, const float* pixeldata, bool filter, bool clampToEdgeS, bool clampToEdgeT);

void TexImage3DPointer_SGL(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR);
void SparseTexImage3DPointer_SGL(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR);
void TexCoordRemap3D_SGL(int axis, int n, const float* values);
void EvalFragmentShader_SGL();
void SetCallback_SGL(bool (*callback)(int progress, int y, int height, void* userptr), void* userptr);
void SetScreenParallel_SGL(bool enable, bool mergeEnable);

void Finish_SGL();

#ifdef HIVE_BUILD_WITH_OPENGL
#define USE_OPENGL
#endif

#ifndef USE_OPENGL
#define BLANKFUNC {}
#define BLANKFUNC_BOOL {return false;}
#define CMDINLINE inline
#else
#define BLANKFUNC
#define BLANKFUNC_BOOL
#define CMDINLINE
#endif

CMDINLINE void CreateBuffer_GL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer, int colorbit, unsigned int& depthRenderbuffer, int depthbit) BLANKFUNC;
CMDINLINE void ReleaseBuffer_GL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer) BLANKFUNC;
CMDINLINE void Clear_GL(float red, float green, float blue, float alpha) BLANKFUNC;
CMDINLINE void GetColorBuffer_GL(int w, int h, unsigned char* imgbuf, int colorbit) BLANKFUNC;
CMDINLINE void GetDepthBuffer_GL(int w, int h, float* depthbuf) BLANKFUNC;
CMDINLINE void CreateFloatBuffer_GL(unsigned int num, float* buffer, unsigned int& buf_id) BLANKFUNC;
CMDINLINE void CreateUintBuffer_GL(unsigned int num, unsigned int* buffer, unsigned int& buf_id) BLANKFUNC;
CMDINLINE void CreateVec4Buffer_GL(unsigned int num, float* buffer, unsigned int& buf_id) BLANKFUNC;
CMDINLINE void CreateVec3Buffer_GL(unsigned int num, float* buffer, unsigned int& buf_id) BLANKFUNC;
CMDINLINE void CreateVec2Buffer_GL(unsigned int num, float* buffer, unsigned int& buf_id) BLANKFUNC;

CMDINLINE void CreateVBIB_GL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
				   unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id, unsigned int& tex_id, unsigned int& index_id) BLANKFUNC;
CMDINLINE void ReleaseBufferVBIB_GL(unsigned int buffer_id) BLANKFUNC;
CMDINLINE void CreateVBRM_GL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
						  unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) BLANKFUNC;
CMDINLINE void CreateVBIBRM_GL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                               unsigned int indexnum, unsigned int* indexbuffer,
                               unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id) BLANKFUNC;

CMDINLINE void BindProgram_GL(unsigned int prg) BLANKFUNC;
CMDINLINE void SetUniform1i_GL(unsigned int prg, const char* name, int val) BLANKFUNC;
CMDINLINE void SetUniform1f_GL(unsigned int prg, const char* name, float val) BLANKFUNC;
CMDINLINE void SetUniform2fv_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetUniform3fv_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetUniform4fv_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetUniformFloatArray_GL(unsigned int prg, const char* name, const float* val, int num) BLANKFUNC;
CMDINLINE void SetUniformMatrix_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetCamera_GL(unsigned int prg, const float* eye, const float* lookat, const float* up, float fov) BLANKFUNC;
CMDINLINE void DrawElements_GL(unsigned int indexnum) BLANKFUNC;
CMDINLINE void DrawLineElements_GL(unsigned int indexnum) BLANKFUNC;
CMDINLINE void DrawArrays_GL(unsigned int indexnum) BLANKFUNC;
CMDINLINE void DrawLineArrays_GL(unsigned int vertexnum) BLANKFUNC;
CMDINLINE void DrawPointArrays_GL(unsigned int vertexnum) BLANKFUNC;
CMDINLINE void DrawTetraArrays_GL(unsigned int vertexnum) BLANKFUNC;
CMDINLINE bool CreateProgramSrc_GL(const char* srcname, unsigned int& prg) BLANKFUNC_BOOL;
CMDINLINE void DrawSolidArrays_GL(int solidType, unsigned int vertexnum) BLANKFUNC;
CMDINLINE bool CreateProgramBinary_GL(const char* soname, unsigned int& prg) BLANKFUNC_BOOL;
CMDINLINE bool DeleteProgram_GL(unsigned int prg) BLANKFUNC_BOOL;
CMDINLINE void BindBufferFloat_GL(unsigned int prg, const char* attrname, unsigned int bufidx) BLANKFUNC;
CMDINLINE void BindBufferUint_GL(unsigned int prg, const char* attrname, unsigned int bufidx) BLANKFUNC;
CMDINLINE void BindBufferVec4_GL(unsigned int prg, const char* attrname, unsigned int bufidx) BLANKFUNC;
CMDINLINE void BindBufferVec3_GL(unsigned int prg, const char* attrname, unsigned int bufidx) BLANKFUNC;
CMDINLINE void BindBufferVec2_GL(unsigned int prg, const char* attrname, unsigned int bufidx) BLANKFUNC;
CMDINLINE void BindVBIB_GL(unsigned int prg, unsigned int vtxidx, unsigned int normalidx, unsigned int matidx, unsigned int texidx, unsigned int indexidx) BLANKFUNC;
CMDINLINE void BindLineVBIB_GL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) BLANKFUNC;
CMDINLINE void BindPointVB_GL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) BLANKFUNC;
CMDINLINE void BindTetraVBIB_GL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) BLANKFUNC;
CMDINLINE void BindFramebuffer_GL(unsigned int frame) BLANKFUNC;
CMDINLINE void LineWidth_GL(float w) BLANKFUNC;
CMDINLINE void GenTextures_GL(int n, unsigned int* tex) BLANKFUNC;
CMDINLINE void DeleteTextures_GL(int n, unsigned int* tex) BLANKFUNC;
CMDINLINE void BindTexture2D_GL(unsigned int tex) BLANKFUNC;
CMDINLINE void ActiveTexture_GL(unsigned int num) BLANKFUNC;
CMDINLINE void TexImage2D_GL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata, bool filter, bool clampToEdgeS, bool clampToEdgeT) BLANKFUNC;
CMDINLINE void TexImage2DFloat_GL(unsigned int width, unsigned int height, unsigned int component, const float* pixeldata, bool filter, bool clampToEdgeS, bool clampToEdgeT) BLANKFUNC;


#endif /* defined(__AnimTool__Commands__) */
