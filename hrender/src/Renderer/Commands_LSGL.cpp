//
//  Commands_GL.cpp
//  AnimTool
//
//  Created by kioku on 13/02/08.
//

#include "Commands.h"

#include <GLES2/gl2.h>
#include <gles_context.h>
#include "../Core/vxmath.h"

void CreateBuffer_SGL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer,unsigned int& depthRenderbuffer)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenFramebuffers(1, &framebuffer);
	sgl.glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	// create color renderbuffer and attach
	sgl.glGenRenderbuffers(1, &colorRenderbuffer);
	sgl.glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, w, h);
	sgl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	
	// create depth renderbuffer and attach
	sgl.glGenRenderbuffers(1, &depthRenderbuffer);
	sgl.glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32_OES, w, h);
	sgl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
	sgl.glViewport(0, 0, w, h);
}


void ReleaseBuffer_SGL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDeleteRenderbuffers(1, &colorRenderbuffer);
	sgl.glDeleteRenderbuffers(1, &depthRenderbuffer);
	sgl.glDeleteFramebuffers(1, &framebuffer);
}


void Clear_SGL(float red, float green, float blue, float alpha)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glClearDepthf(10000.0);
	sgl.glClearColor(red, green, blue, alpha);
	sgl.glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


void GetColorBuffer_SGL(int w, int h, unsigned char* imgbuf)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glFinish();
	sgl.glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, imgbuf);
}

void GetDepthBuffer_SGL(int w, int h, float* depthbuf)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depthbuf);
}

void CreateVBIB_SGL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* materialbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
					unsigned int& vtx_id, unsigned int& normal_id, unsigned int& material_id, unsigned int& tex_id, unsigned int& index_id) {
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenBuffers(1, &vtx_id);
	sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_id);
	sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, posbuffer, GL_STATIC_DRAW);
	sgl.glGenBuffers(1, &normal_id);
	sgl.glBindBuffer(GL_ARRAY_BUFFER, normal_id);
	sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, normalbuffer, GL_STATIC_DRAW);
	sgl.glGenBuffers(1, &material_id);
	sgl.glBindBuffer(GL_ARRAY_BUFFER, material_id);
	sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, materialbuffer, GL_STATIC_DRAW);
    if (texbuffer) {
        sgl.glGenBuffers(1, &tex_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, tex_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*vertexnum, texbuffer, GL_STATIC_DRAW);
    }
	if (indexnum > 0) {
		sgl.glGenBuffers(1, &index_id);
		sgl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
		sgl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexnum, indexbuffer, GL_STATIC_DRAW);
	}
}

void CreateVBRM_SGL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
					unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenBuffers(1, &vtx_id);
	sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_id);
	sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, posbuffer, GL_STATIC_DRAW);

    if (radiusbuffer) {
        sgl.glGenBuffers(1, &radius_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, radius_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, radiusbuffer, GL_STATIC_DRAW);
    }
    if (matbuffer) {
        sgl.glGenBuffers(1, &mat_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, mat_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, matbuffer, GL_STATIC_DRAW);
    }
}

void CreateVBIBRM_SGL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                      unsigned int indexnum, unsigned int* indexbuffer,
                    unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glGenBuffers(1, &vtx_id);
    sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_id);
    sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, posbuffer, GL_STATIC_DRAW);
    
    if (radiusbuffer) {
        sgl.glGenBuffers(1, &radius_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, radius_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, radiusbuffer, GL_STATIC_DRAW);
    }
    if (mat_id) {
        sgl.glGenBuffers(1, &mat_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, mat_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, matbuffer, GL_STATIC_DRAW);
    }
    
    sgl.glGenBuffers(1, &index_id);
    sgl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
    sgl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexnum, indexbuffer, GL_STATIC_DRAW);
}

void ReleaseBufferVBIB_SGL(unsigned int buffer_id)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDeleteBuffers(1, &buffer_id);
}

void BindProgram_SGL(unsigned int prg)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glUseProgram(prg);
}

void SetUniform1i_SGL(unsigned int prg, const char* name, int val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform1i(p, val);
}

void SetUniform1f_SGL(unsigned int prg, const char* name, float val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform1f(p, val);
}

void SetUniform2fv_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform2fv(p, 1, val);
}

void SetUniform3fv_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform3fv(p, 1, val);
}

void SetUniform4fv_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform4fv(p, 1, val);
}

void SetUniformFloatArray_SGL(unsigned int prg, const char* name, const float* val, int num)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform1fv(p, num, val);
}

void SetUniformMatrix_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniformMatrix4fv(p, 1, 0, val);
}

void SetCamera_SGL(unsigned int prg, const float* eye, const float* lookat, const float* up, float fov)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglSetCamera(eye, lookat, up, fov);
	
	using namespace VX::Math;
	VX::Math::vec3 dir = VX::Math::vec3(lookat[0]-eye[0],lookat[1]-eye[1],lookat[2]-eye[2]);
	dir = normalize(dir);
	
	SetUniform3fv_SGL(prg, "eyepos", &eye[0]);
	SetUniform3fv_SGL(prg, "eyedir", &dir[0]);
	
	SetUniform3fv_SGL(prg, "eye", &eye[0]);
	SetUniform3fv_SGL(prg, "lookat", &lookat[0]);
	SetUniform3fv_SGL(prg, "up", &up[0]);
}

void SetStereoEnvCamera_SGL(unsigned int prg, const float* eye, const float* lookat, const float* up, float zeroParallax, float eyeSeparation)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglSetStereoEnvCamera(eye, lookat, up, zeroParallax, eyeSeparation);
	
	using namespace VX::Math;
	VX::Math::vec3 dir = VX::Math::vec3(lookat[0]-eye[0],lookat[1]-eye[1],lookat[2]-eye[2]);
	dir = normalize(dir);
	
	SetUniform3fv_SGL(prg, "eyepos", &eye[0]);
	SetUniform3fv_SGL(prg, "eyedir", &dir[0]); // @note { not working }
	
	SetUniform3fv_SGL(prg, "eye", &eye[0]);
	SetUniform3fv_SGL(prg, "lookat", &lookat[0]);
	SetUniform3fv_SGL(prg, "up", &up[0]);
}

void DrawElements_SGL(unsigned int indexnum)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
}

void DrawLineElements_SGL(unsigned int indexnum)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glDrawElements(GL_LINES, indexnum, GL_UNSIGNED_INT, (void*)0);
}

void DrawArrays_SGL(unsigned int vtxnum)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDrawArrays(GL_TRIANGLES, 0, vtxnum);
}

void DrawPointArrays_SGL(unsigned int vtxnum)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDrawArrays(GL_POINTS, 0, vtxnum);
}

void DrawLineArrays_SGL(unsigned int vtxnum)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glDrawArrays(GL_LINES, 0, vtxnum);
}

void SampleCoverage_SGL(float a, bool invert)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glEnable(GL_SAMPLE_COVERAGE);
	sgl.glSampleCoverage(a, invert);
}

void PixelStep_SGL(int n)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglSetPixelStep(n);
}

void SetShaderCompiler_SGL(const char* path, const char* opt)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.lsglSetShaderCompiler(path, opt);
}

bool CreateProgramSrc_SGL(const char* srcname, unsigned int& prg)
{
	static GLchar srcbuf[16384];
	FILE *fp = fopen(srcname, "rb");
	if (!fp){
		printf("Error: Not found Program [%s]\n", srcname);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	rewind(fp);
	len = fread(srcbuf, 1, len, fp);
	srcbuf[len] = 0;
	fclose(fp);
	
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	if (prg != 0)
		sgl.glDeleteProgram(prg);
	prg = 0;
	
	static const GLchar *src = srcbuf;
	unsigned int fragShader;
	fragShader = sgl.glCreateShader(GL_FRAGMENT_SHADER);
	sgl.glShaderSource(fragShader, 1, &src, NULL);
	sgl.glCompileShader(fragShader);
	GLint val = 0;
	sgl.glGetShaderiv(fragShader, GL_COMPILE_STATUS, &val);
	//assert(val == GL_TRUE && "failed to compile shader");
	if (val!=GL_TRUE) {
		printf("Error: failed to compile shader [%s]\n", srcname);
		return false;
	}
	
	prg = sgl.glCreateProgram();
	sgl.glAttachShader(prg, fragShader);
	sgl.glLinkProgram(prg);
	sgl.glGetProgramiv(prg, GL_LINK_STATUS, &val);
	//assert(val == GL_TRUE && "failed to link shader");
	if (val!=GL_TRUE) {
		printf("Error: failed to link shader [%s]\n", srcname);
		return false;
	}
	
	return true;
}

bool CreateProgramBinary_SGL(const char* soname, unsigned int& prg)
{
	std::vector<unsigned char> data;
	FILE *fp = fopen(soname, "rb");
	if (!fp) {
		printf("\nFailed to open shader binary file: %s\n", soname);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	unsigned int len = static_cast<unsigned int>(ftell(fp));
	rewind(fp);
	data.resize(len);
	len = static_cast<unsigned int>(fread(&data.at(0), 1, len, fp));
	fclose(fp);

	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	if (prg != 0)
		sgl.glDeleteProgram(prg);
	
	unsigned int fragShader = sgl.glCreateShader(GL_FRAGMENT_SHADER);
	GLenum format = 0; // format is arbitrary at this time.
	sgl.glShaderBinary(1, &fragShader, format, &data[0], len);
	
	prg = sgl.glCreateProgram();
	sgl.glAttachShader(prg, fragShader);
	sgl.glLinkProgram(prg);
	
	GLint val = 0;
	sgl.glGetShaderiv(fragShader, GL_COMPILE_STATUS, &val);
	assert(val == GL_TRUE && "failed to compile shader");

	return true;
}

bool DeleteProgram_SGL(unsigned int prg)
{
	assert(prg);
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDeleteProgram(prg);
	return true;
}

void BindVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int normalidx, unsigned int vtx_material, unsigned int texidx, unsigned int indexidx)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	GLint attrMaterial = sgl.glGetAttribLocation(prg, "matID");
	GLint attrNormal = sgl.glGetAttribLocation(prg, "mnormal");
	GLint attrPos    = sgl.glGetAttribLocation(prg, "position");
    GLint attrTex    = sgl.glGetAttribLocation(prg, "texcoord");
    if (attrTex != -1 && texidx != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, texidx);
        sgl.glVertexAttribPointer(attrTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
        sgl.glEnableVertexAttribArray(attrTex);
    }
	if (attrMaterial != -1) {
		sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_material);
		sgl.glVertexAttribPointer(attrMaterial, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		sgl.glEnableVertexAttribArray(attrMaterial);
	}
	
	if (attrNormal != -1) {
		sgl.glBindBuffer(GL_ARRAY_BUFFER, normalidx);
		sgl.glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		sgl.glEnableVertexAttribArray(attrNormal);
	}

	if (attrPos != -1) {
		sgl.glBindBuffer(GL_ARRAY_BUFFER, vtxidx);
		sgl.glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		sgl.glEnableVertexAttribArray(attrPos);
	}
    sgl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexidx);
}

void BindPointVB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	GLint attrRadius   = sgl.glGetAttribLocation(prg, "lsgl_PointSize");
	GLint attrMaterial = sgl.glGetAttribLocation(prg, "matID");
	GLint attrPos      = sgl.glGetAttribLocation(prg, "position");
	
	if (attrRadius != -1) {
		sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_radius);
		sgl.glVertexAttribPointer(attrRadius, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		sgl.glEnableVertexAttribArray(attrRadius);
	}
	if (attrMaterial != -1) {
		sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_material);
		sgl.glVertexAttribPointer(attrMaterial, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		sgl.glEnableVertexAttribArray(attrMaterial);
	}
	if (attrPos != -1) {
		sgl.glBindBuffer(GL_ARRAY_BUFFER, vtxidx);
		sgl.glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		sgl.glEnableVertexAttribArray(attrPos);
	}
}

void BindLineVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attrRadius   = sgl.glGetAttribLocation(prg, "lsgl_LineWidth");
    GLint attrMaterial = sgl.glGetAttribLocation(prg, "matID");
    GLint attrPos      = sgl.glGetAttribLocation(prg, "position");
    
    if (attrRadius != -1 && vtx_radius > 0) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_radius);
        sgl.glVertexAttribPointer(attrRadius, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        sgl.glEnableVertexAttribArray(attrRadius);
    }
    if (attrMaterial != -1 && vtx_material > 0) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_material);
        sgl.glVertexAttribPointer(attrMaterial, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        sgl.glEnableVertexAttribArray(attrMaterial);
    }
    if (attrPos != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, vtxidx);
        sgl.glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
        sgl.glEnableVertexAttribArray(attrPos);
    }

    sgl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexidx);
}

void LineWidth_SGL(float w)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glLineWidth(w);
}

void GenTextures_SGL(int n, unsigned int* tex)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenTextures(n, tex);
}
void BindTexture3D_SGL(unsigned int voltex)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glBindTexture(GL_TEXTURE_3D, voltex);
}

void BindTexture2D_SGL(unsigned int texid)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glBindTexture(GL_TEXTURE_2D, texid);
}

void ActiveTexture_SGL(unsigned int n)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glActiveTexture(GL_TEXTURE0 + n);
}

void TexImage2D_SGL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	GLint format = GL_LUMINANCE;
	if      (component == 3) format = GL_RGB;
	else if (component == 4) format = GL_RGBA;
	else if (component == 1) format = GL_LUMINANCE;
	else {
		assert(0);
	}
	sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	sgl.glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixeldata);
}

void TexImage3DPointer_SGL(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	GLint format = GL_LUMINANCE;
	if (component == 4) {
		format = GL_RGBA; 
	} else if (component == 3) {
		format = GL_RGB; // as vector
	} else if (component == 1){
		format = GL_LUMINANCE;
	} else {
		assert(0);
	}
	sgl.lsglTexImage3DPointer(GL_TEXTURE_3D, 0, format, width, height, depth, 0, format, GL_FLOAT, volumedata);
}

void Finish_SGL()
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glFinish();
}

void EvalFragmentShader_SGL()
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglEvalFragmentShader();
}

void SetCallback_SGL(LSGLProgressCallback callback)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglSetProgressCallback(callback);
}
