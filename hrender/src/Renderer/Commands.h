//
//  Commands.h
//  AnimTool
//
//  Created by kioku on 13/02/08.
//

#ifndef __AnimTool__Commands__
#define __AnimTool__Commands__

void CreateBuffer_LSGL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer,unsigned int& depthRenderbuffer);
void ReleaseBuffer_SGL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer);
void Clear_SGL(float red, float green, float blue, float alpha);
void GetColorBuffer_SGL(int w, int h, unsigned char* imgbuf);
void GetDepthBuffer_SGL(int w, int h, float* depthbuf);

void CreateVBIB_SGL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
					unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id, unsigned int& tex_id, unsigned int& index_id);
void ReleaseBufferVBIB_SGL(unsigned int buffer_id);
void CreateVBRM_SGL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
					unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id);

void BindProgram_SGL(unsigned int prg);
void SetUniform1i_SGL(unsigned int prg, const char* name, int val);
void SetUniform1f_SGL(unsigned int prg, const char* name, const float val);
void SetUniform2fv_SGL(unsigned int prg, const char* name, const float* val);
void SetUniform3fv_SGL(unsigned int prg, const char* name, const float* val);
void SetUniform4fv_SGL(unsigned int prg, const char* name, const float* val);
void SetUniformFloatArray_SGL(unsigned int prg, const char* name, const float* val, int num);
void SetUniformMatrix_SGL(unsigned int prg, const char* name, const float* val);
void SetCamera_SGL(unsigned int prg, float* eye, float* lookat, float* up, float fov);
void SetStereoEnvCamera_SGL(unsigned int prg, float* eye, float* lookat, float* up, float zeroParallax, float eyeSeparation);
void DrawElements_SGL(unsigned int indexnum);
void DrawArrays_SGL(unsigned int vtxnum);
void DrawPointArrays_SGL(unsigned int vertexnum);
void SampleCoverage_SGL(float a, bool invert);
void PixelStep_SGL(int n);
bool CreateProgramSrc_SGL(const char* srcname, unsigned int& prg);
bool CreateProgramBinary_SGL(const char* soname, unsigned int& prg);
bool DeleteProgram_SGL(unsigned int prg);
void BindVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int normalidx, unsigned int matidx, unsigned int texidx, unsigned int indexidx);
void BindPointVB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material);

void GenTextures_SGL(int n, unsigned int* tex);
void BindTexture3D_SGL(unsigned int tex);
void BindTexture2D_SGL(unsigned int tex);
void ActiveTexture_SGL(unsigned int n);
void TexImage2D_SGL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata);

void TexImage3DPointer_SGL(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata);
void EvalFragmentShader_SGL();
void SetCallback_SGL(bool (*callback)(int progress, int y, int height) );
void Finish_SGL();

#ifndef USE_OPENGL
#define BLANKFUNC {}
#define BLANKFUNC_BOOL {return false;}
#define CMDINLINE inline
#else
#define BLANKFUNC
#define BLANKFUNC_BOOL
#define CMDINLINE
#endif

CMDINLINE void CreateBuffer_GL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer,unsigned int& depthRenderbuffer) BLANKFUNC;
CMDINLINE void ReleaseBuffer_GL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer) BLANKFUNC;
CMDINLINE void Clear_GL(float red, float green, float blue, float alpha) BLANKFUNC;
CMDINLINE void GetColorBuffer_GL(int w, int h, unsigned char* imgbuf) BLANKFUNC;
CMDINLINE void GetDepthBuffer_GL(int w, int h, float* depthbuf) BLANKFUNC;

CMDINLINE void CreateVBIB_GL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
				   unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id, unsigned int& tex_id, unsigned int& index_id) BLANKFUNC;
CMDINLINE void ReleaseBufferVBIB_GL(unsigned int buffer_id) BLANKFUNC;
CMDINLINE void CreateVBRM_GL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
						  unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) BLANKFUNC;

CMDINLINE void BindProgram_GL(unsigned int prg) BLANKFUNC;
CMDINLINE void SetUniform1i_GL(unsigned int prg, const char* name, int val) BLANKFUNC;
CMDINLINE void SetUniform1f_GL(unsigned int prg, const char* name, float val) BLANKFUNC;
CMDINLINE void SetUniform2fv_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetUniform3fv_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetUniform4fv_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetUniformFloatArray_GL(unsigned int prg, const char* name, const float* val, int num) BLANKFUNC;
CMDINLINE void SetUniformMatrix_GL(unsigned int prg, const char* name, const float* val) BLANKFUNC;
CMDINLINE void SetCamera_GL(unsigned int prg, float* eye, float* lookat, float* up, float fov, int w, int h, float nearVal, float farVal) BLANKFUNC;
CMDINLINE void DrawElements_GL(unsigned int indexnum) BLANKFUNC;
CMDINLINE void DrawArrays_GL(unsigned int indexnum) BLANKFUNC;
CMDINLINE void DrawPointArrays_GL(unsigned int vertexnum) BLANKFUNC;
CMDINLINE bool CreateProgramSrc_GL(const char* srcname, unsigned int& prg, bool usePointShader) BLANKFUNC_BOOL;
CMDINLINE bool CreateProgramBinary_GL(const char* soname, unsigned int& prg) BLANKFUNC_BOOL;
CMDINLINE bool DeleteProgram_GL(unsigned int prg) BLANKFUNC_BOOL;
CMDINLINE void BindVBIB_GL(unsigned int prg, unsigned int vtxidx, unsigned int normalidx, unsigned int matidx, unsigned int texidx, unsigned int indexidx) BLANKFUNC;
CMDINLINE void BindPointVB_GL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) BLANKFUNC;
CMDINLINE void BindFramebuffer_GL(unsigned int frame) BLANKFUNC;
CMDINLINE void GenTextures_GL(int n, unsigned int* tex) BLANKFUNC;
CMDINLINE void BindTexture2D_GL(unsigned int tex) BLANKFUNC;
CMDINLINE void ActiveTexture_GL(unsigned int num) BLANKFUNC;
CMDINLINE void TexImage2D_GL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata) BLANKFUNC;


#endif /* defined(__AnimTool__Commands__) */
