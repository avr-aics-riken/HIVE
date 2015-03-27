//
//  Commands_GL.cpp
//  AnimTool
//
//  Created by kioku on 13/02/08.
//
/**
 * @file Commands_GL.cpp
 * GLコマンド
 */
#include "Commands.h"

#ifdef USE_OPENGL

#include "Graphics.h"
#include <stdio.h>
#include "Math.h"

namespace  {
	float g_near = 10.0;
	float g_far  = 10000.0;
}

/// カレントコンテキストの取得.
VX::Graphics& GetCurrentGraphics()
{
	static VX::Graphics g;
	return g;
}

/**
 * GLバッファの作成
 * @param w 幅
 * @param h 高さ
 * @param framebuffer フレームバッファ
 * @param colorRenderbuffer カラーレンダーバッファ
 * @param despthRenderBuffer デプスレンダーバッファ
 */
void CreateBuffer_GL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer,unsigned int& depthRenderbuffer)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.GenFramebuffers(1, &framebuffer);
	g.BindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	// create color renderbuffer and attach
	g.GenRenderbuffers(1, &colorRenderbuffer);
	g.BindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
	g.RenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h);
	g.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	
	// create depth renderbuffer and attach
	g.GenRenderbuffers(1, &depthRenderbuffer);
	g.BindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	g.RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
	g.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
	g.Viewport(0, 0, w, h);
}
/**
 * GLバッファの解放
 * @param framebuffer フレームバッファ
 * @param colorRenderbuffer カラーレンダーバッファ
 * @param depthRenderbuffer 深度レンダーバッファ
 */
void ReleaseBuffer_GL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.DeleteRenderbuffers(1, &colorRenderbuffer);
	g.DeleteRenderbuffers(1, &depthRenderbuffer);
	g.DeleteFramebuffers(1, &framebuffer);
	
}
/**
 * GLクリア
 * @param red 赤
 * @param green 緑
 * @param blue 青
 * @param alpha 透明度
 */
void Clear_GL(float red, float green, float blue, float alpha)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.ClearColor(red, green, blue, alpha);
	g.Clear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
/**
 * GLピクセル値取得
 * @param w 幅
 * @param h 高さ
 * @param imgbuf 結果を格納するバッファ
 */
void GetColorBuffer_GL(int w, int h, unsigned char* imgbuf)
{
  //glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  //glPixelStorei(GL_PACK_ALIGNMENT,1);
	VX::Graphics& g = GetCurrentGraphics();
	g.ReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, imgbuf);
	g.Finish();
}

/**
 * GLデプスバッファ値取得.
 * @param w 幅
 * @param h 高さ
 * @param imgbuf 結果を格納するバッファ
 */
void GetDepthBuffer_GL(int w, int h, float* depthbuf)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.Finish();
	g.ReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depthbuf);
	g.Finish();
	for (int y = 0; y < h; ++y){
		for (int x = 0; x < w; ++x){
			const float d = depthbuf[x + y * w];
			const float f = g_far;
			const float n = g_near;
			depthbuf[x + y * w] = -(f * n / (d * (f - n) - f));
		}
	}

}

/**
 * GLバッファの作成
 * @param vertexnum 頂点数
 * @param posbuffer 頂点バッファ
 * @param normalbuffer 法線バッファ
 * @param matbuffer マテリアルバッファ
 * @param texbuffer テクスチャバッファ
 * @param indexnum インデックス数
 * @param indexbuffer インデックスバッファ
 * @param vtx_id 頂点ID
 * @param normal_id 法線ID
 * @param mat_id マテリアルID
 * @param tex_id テクスチャID
 * @param index_id インデックスID
 */
void CreateVBIB_GL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
				   unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id, unsigned int& tex_id, unsigned int& index_id)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.GenBuffers(1, &vtx_id);
	g.BindBuffer(GL_ARRAY_BUFFER, vtx_id);
	g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, posbuffer, GL_STATIC_DRAW);
	g.GenBuffers(1, &normal_id);
	g.BindBuffer(GL_ARRAY_BUFFER, normal_id);
	g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, normalbuffer, GL_STATIC_DRAW);
	g.GenBuffers(1, &mat_id);
	g.BindBuffer(GL_ARRAY_BUFFER, mat_id);
	g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, matbuffer, GL_STATIC_DRAW);
    if (texbuffer) {
        g.GenBuffers(1, &tex_id);
        g.BindBuffer(GL_ARRAY_BUFFER, tex_id);
        g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*2*vertexnum, texbuffer, GL_STATIC_DRAW);
    }
	if (indexnum > 0) {
		g.GenBuffers(1, &index_id);
		g.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
		g.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexnum, indexbuffer, GL_STATIC_DRAW);
	}
}

/**
 * GLバッファの作成
 * @param vertexnum 頂点数
 * @param posbuffer 頂点バッファ
 * @param radiusbuffer 半径バッファ
 * @param matbuffer マテリアルバッファ
 * @param vtx_id 頂点ID
 * @param radius_id 半径ID
 * @param mat_id マテリアルID
 */
void CreateVBRM_GL(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
				   unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.GenBuffers(1, &vtx_id);
	g.BindBuffer(GL_ARRAY_BUFFER, vtx_id);
	g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, posbuffer, GL_STATIC_DRAW);
	g.GenBuffers(1, &radius_id);
	g.BindBuffer(GL_ARRAY_BUFFER, radius_id);
	g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, radiusbuffer, GL_STATIC_DRAW);
	g.GenBuffers(1, &mat_id);
	g.BindBuffer(GL_ARRAY_BUFFER, mat_id);
	g.BufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, matbuffer, GL_STATIC_DRAW);
	//g.GenBuffers(1, &index_id);
	//g.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
	//g.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexnum, indexbuffer, GL_STATIC_DRAW);
}

/**
 * GLバッファの解放
 * @param buffer_id バッファID
 */
void ReleaseBufferVBIB_GL(unsigned int buffer_id)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.DeleteBuffers(1, &buffer_id);
}

/**
 * シェーダプログラムのバインド
 * @param prg シェーダプログラムID
 */
void BindProgram_GL(unsigned int prg)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.UseProgram(prg);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform1i_GL(unsigned int prg, const char* name, int val)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.Uniform1i(p, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform1f_GL(unsigned int prg, const char* name, float val)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.Uniform1f(p, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform2fv_GL(unsigned int prg, const char* name, const float* val)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.Uniform2fv(p, 1, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform3fv_GL(unsigned int prg, const char* name, const float* val)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.Uniform3fv(p, 1, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform4fv_GL(unsigned int prg, const char* name, const float* val)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.Uniform4fv(p, 1, val);
}

/**
 * Uniform値の設定(配列)
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 * @param num 個数.
 */
void SetUniformFloatArray_GL(unsigned int prg, const char* name, const float* val, int num)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.Uniform1fv(p, num, val);
}

/**
 * Uniform値の設定(行列)
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniformMatrix_GL(unsigned int prg, const char* name, const float* val)
{
	VX::Graphics& g = GetCurrentGraphics();
	const int p = g.GetUniformLocation(prg, name);
	if (p != -1)
		g.UniformMatrix4fv(p, 1, 0, val);
}

/**
 * GLカメラの設定
 * @param prg シェーダプログラムID
 * @param eye 視点
 * @param lookat 注目点
 * @param up アップベクトル
 * @param fov 視野角
 * @param w 幅
 * @param h 高さ
 * @param nearVal ニア
 * @param farVal ファー
 */
void SetCamera_GL(unsigned int prg, float* eye, float* lookat, float* up, float fov, int w, int h, float nearVal, float farVal)
{
	g_near = nearVal;
	g_far  = farVal;
	using namespace VX::Math;
	matrix view = LookAt(eye, lookat, up);
	matrix proj = PerspectiveFov(fov, w/static_cast<float>(h), nearVal, farVal);
	VX::Graphics& g = GetCurrentGraphics();
	const int vp = g.GetUniformLocation(prg, "view");
	if (vp != -1)
		g.UniformMatrix4fv(vp, 1, false, &view.f[0]);
	const int pp = g.GetUniformLocation(prg, "proj");
	if (pp != -1)
		g.UniformMatrix4fv(pp, 1, false, &proj.f[0]);
}

/// インデックスで描画
/// @param indexnum インデックス数
void DrawElements_GL(unsigned int indexnum)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.DrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
}

/// 三角形の描画.
/// @param vertexnum 頂点数.
void DrawArrays_GL(unsigned int vertexnum)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.DrawArrays(GL_TRIANGLES, 0, vertexnum);
}

/// 点の描画.
/// @param vertexnum 頂点数.
void DrawPointArrays_GL(unsigned int vertexnum)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.DrawArrays(GL_POINTS, 0, vertexnum);
}

#define STRINGIFY(A) #A
#define STRINGIFY2(A) "#version 120\n"#A

static const char* g_vertsrc = STRINGIFY(
										 attribute vec3 position;
										 attribute vec3 mnormal;
										 attribute float materialID;
										 uniform mat4 proj;
										 uniform mat4 view;
										 uniform mat4 lsgl_World;
										 uniform vec3 offset;
										 varying vec3 normal;
										 varying vec3 vnormal;
										 varying vec3 eyedir;
										 varying float matID;
										 void main(void)
										 {
											 normal = normalize(lsgl_World * vec4(mnormal,0.0)).xyz;
											 vnormal = normalize(view * vec4(normal,0.0)).xyz;
											 vec4 vpos = view * lsgl_World * vec4(position,1);
											 eyedir = normalize(vpos.xyz);
											 matID = materialID;
											 gl_Position = proj * vpos;
										 }
										 );

static const char* g_pointvertsrc = STRINGIFY2(
										 attribute vec3 position;
										 attribute float lsgl_PointSize;
										 attribute float materialID;
										 uniform mat4 proj;
										 uniform mat4 view;
									     uniform mat4 lsgl_World;
										 varying float radius;
										 varying float matID;
										 varying vec4 color;
										 varying vec3 eyedir;
										 vec3 matToColor(float m)
										 {
											// NOTE: Replace following code with 1D texture fetch would be better idea!
											
											if (m < 0.0) { // Unsupported material color
												return vec3(0.75, 0.75, 1.0);
											}
											
											 float lt = 1.0;
											 //if (m > 20000.0) {
											 //	 lt = 1.0;
											 //	 m = m - 20000.0;
											 // }
											 
											 if (m < 1.1) { // Hydrogen
												 return vec3(1.0, 1.0, 1.0)*vec3(lt);
											 } else if (m < 2.1) { // Hellium
												 return vec3(0.85, 1.0, 1.0)*vec3(lt);
											 } else if (m < 3.1) { // Lithium
												 return vec3(0.8, 0.5, 1.0)*vec3(lt);
											 } else if (m < 4.1) { // Beryllium
												 return vec3(0.76, 1.0, 0.0)*vec3(lt);
											 } else if (m < 5.1) { // Boron
												 return vec3(1.0, 0.7, 0.7)*vec3(lt);
											 } else if (m < 6.1) { // Carbon
												 return vec3(0.56, 0.56, 0.56)*vec3(lt);
											 } else if (m < 7.1) { // Nitrogen
												 return vec3(0.18, 0.31, 0.97)*vec3(lt);
											 } else if (m < 8.1) { // Oxygen
												 return vec3(1.0, 0.05, 0.05)*vec3(lt);
											 } else if (m < 9.1) { // Fluorine
												 return vec3(0.56, 0.87, 0.31)*vec3(lt);
											 } else if (m < 10.1) { // Neon
												 return vec3(0.7, 0.89, 0.96)*vec3(lt);
											 } else if (m < 11.1) { // Sodium
												 return vec3(0.67, 0.36, 0.94)*vec3(lt);
											 } else if (m < 12.1) { // Magnesium
												 return vec3(0.54, 1.0, 0.0)*vec3(lt);
											 } else if (m < 13.1) { // Aluminium
												 return vec3(0.74, 065, 0.65)*vec3(lt);
											 } else if (m < 14.1) { // Silicon
												 return vec3(0.94, 0.78, 0.62)*vec3(lt);
											 } else if (m < 15.1) { // Phosphorus
												 return vec3(1.0, 0.5, 0.0)*vec3(lt);
											 } else if (m < 16.1) { // Sulfur
												 return vec3(1.0, 1.0, 0.18)*vec3(lt);
											 } else if (m < 11000.0) { // H2)
												 return vec3(0.5, 0.5, 0.931)*vec3(lt);
											 }
											 return vec3(0.0, 1.0, 0.0);
										 }
										 void main(void)
										 {
											 radius = lsgl_PointSize;
											 matID  = materialID;
											 color.rgb = matToColor(matID);
											 color.a = 1.0;
											 vec4 vpos = view * lsgl_World * vec4(position,1);
											 eyedir = normalize(vpos.xyz);
											 vec4 p = proj * vpos;
											 gl_Position = p;
											 gl_PointSize = radius*700.0/p.w;// * vpos.z;
										 }
										 );

/**
 * シェーダ情報ログのプリント
 * @param g グラフィックコンテキスト
 * @param shader シェーダID
 */
void printShaderInfoLog(VX::Graphics* g, GLuint shader)
{
	int bufSize = 0;
	g->GetShaderiv(shader, VG_INFO_LOG_LENGTH , &bufSize);
	printf("ShaderError=%d\n",bufSize);
	if (bufSize > 1)
	{
		char *infoLog;
		infoLog = new char[bufSize];
		if (infoLog != NULL)
		{
			int length;
			g->GetShaderInfoLog(shader, bufSize, &length, infoLog);
			//cout << "InfoLog:" << endl << infoLog << endl << endl;
			printf("InfoLog:\n%s\n",infoLog);
			delete [] infoLog;
		}
		else
		{
			//cout << "Could not allocate InfoLog buffer." << endl;
			printf("Could not allocate InfoLog buffer.");
		}
	}
}

/**
 * プログラム情報ログのプリント
 * @param g グラフィックコンテキスト
 * @param shader シェーダプログラムID
 */
void printProgramInfoLog(VX::Graphics* g, GLuint program)
{
	int bufSize;
	g->GetProgramiv(program, VG_INFO_LOG_LENGTH , &bufSize);
	if (bufSize > 1)
	{
		char *infoLog;
		infoLog = new char[bufSize];
		if (infoLog != NULL)
		{
			int length;
			g->GetProgramInfoLog(program, bufSize, &length, infoLog);
			//cout << "InfoLog:" << endl << infoLog << endl << endl;
			printf("InfoLog:\n%s\n",infoLog);
			delete [] infoLog;
		}
		else
		{
			//cout << "Could not allocate InfoLog buffer." << endl;
			printf("Could not allocate InfoLog buffer.");
		}
	}
}

/**
 * 文字列の検索.
 * @param str1 検索対象文字列.
 * @param str2 検索文字列.
 */
bool findstr(const char* str1, const char* str2)
{
	char *cp = (char *) str1;
	char *s1, *s2;

	if ( !*str2 )
		return false;
	while (*cp){
		s1 = cp;
		s2 = (char *) str2;
		while ( *s1 && *s2 && !(*s1-*s2) )
			s1++, s2++;
		if (!*s2)
			return true;
		cp++;
	}
	return false;
}

/**
 * シェーダプログラムの作成.
 * @param srcname　シェーダパス
 * @param [out] prg シェーダプログラムID.
 * @param usePointShader 点シェーダを使うかどうか.
 * @retval true 成功.
 * @retval false 失敗.
 */
bool CreateProgramSrc_GL(const char* srcname, unsigned int& prg, bool usePointShader)
{
	static GLchar srcbuf[16384];
	char fbuf[1024] = {};
	//sprintf(fbuf, "%s.gl", srcname); // old specification (add .gl)
	sprintf(fbuf, "%s", srcname);
	printf("LOAD GL SHADER:%s\n",fbuf);
	FILE *fp = fopen(fbuf, "rb");
	//assert(fp);
	if (!fp) {
		fprintf(stderr, "Can't open : %s\n",fbuf);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	rewind(fp);
	len = fread(srcbuf, 1, len, fp);
	srcbuf[len] = 0;
	fclose(fp);
	
	VX::Graphics& g = GetCurrentGraphics();
	if (prg != 0)
		g.DeleteProgram(prg);
	
	g.Enable(GL_VERTEX_PROGRAM_POINT_SIZE);
	g.Enable(GL_POINT_SPRITE);
	
	if (findstr(srcbuf, "gl_PointCoord"))
		usePointShader = true;
	
	unsigned int vertShader = g.CreateShader(GL_VERTEX_SHADER);
	if (usePointShader)
		g.ShaderSource(vertShader, 1, &g_pointvertsrc, NULL);
	else
		g.ShaderSource(vertShader, 1, &g_vertsrc, NULL);
	g.CompileShader(vertShader);
	GLint val = 0;
	g.GetShaderiv(vertShader, GL_COMPILE_STATUS, &val);
	if (val != GL_TRUE)
		printShaderInfoLog(&g, vertShader);
	printf("VShader compiled=%d\n",val);
	//assert(val == GL_TRUE && "failed to compile shader");
	if (val!=GL_TRUE) {
		fprintf(stderr, "Error: failed to compile shader [%s]\n", srcname);
		return false;
	}
	
	static const GLchar *src = srcbuf;
	unsigned int fragShader = g.CreateShader(GL_FRAGMENT_SHADER);
	g.ShaderSource(fragShader, 1, &src, NULL);
	g.CompileShader(fragShader);
	g.GetShaderiv(fragShader, GL_COMPILE_STATUS, &val);
	if (val != GL_TRUE)
		printShaderInfoLog(&g, fragShader);
	printf("FShader compiled=%d\n",val);
	//assert(val == GL_TRUE && "failed to compile shader");
	if (val!=GL_TRUE) {
		fprintf(stderr, ("Error: failed to compile shader [%s]\n", srcname);
		return false;
	}
	
	prg = g.CreateProgram();
	g.AttachShader(prg, vertShader);
	g.AttachShader(prg, fragShader);
	g.LinkProgram(prg);
	g.GetProgramiv(prg, GL_LINK_STATUS, &val);
	if (val != GL_TRUE)
		printProgramInfoLog(&g, prg);
	//assert(val == GL_TRUE && "failed to link shader");
	if (val!=GL_TRUE) {
		fprintf(stderr, "Error: failed to compile shader [%s]\n", srcname);
		return false;
	}
	
	g.DeleteShader(vertShader);
	g.DeleteShader(fragShader);
	
	return true;
}

bool CreateProgramBinary_GL(const char* soname, unsigned int& prg)
{
	// TODO
	assert(0);
	return false;
}

/**
 * シェーダプログラムの削除.
 * @param prg シェーダプログラムID
 */
bool DeleteProgram_GL(unsigned int prg)
{
	//assert(prg);
	if (prg > 0)
		glDeleteProgram(prg);
	return true;
}

/**
 * バッファのバインド.
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param normalidx 法線インデックスバッファID
 * @param matidx マテリアルインデックスバッファID
 * @param texidx テクスチャインデックスバッファID
 * @param indexidx インデックスバッファID
 */
void BindVBIB_GL(unsigned int prg, unsigned int vtxidx, unsigned int normalidx, unsigned int matidx, unsigned int texidx, unsigned int indexidx)
{
	VX::Graphics& g = GetCurrentGraphics();
	GLint attrMaterial = g.GetAttribLocation(prg, "materialID");
	GLint attrNormal = g.GetAttribLocation(prg, "mnormal");
    GLint attrPos    = g.GetAttribLocation(prg, "position");
    GLint attrTex    = g.GetAttribLocation(prg, "texcoord");
    if (attrTex >= 0 && texidx > 0) {
        g.BindBuffer(GL_ARRAY_BUFFER, texidx);
        g.VertexAttribPointer(attrTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
        g.EnableVertexAttribArray(attrTex);
    }
	if (attrMaterial >= 0) {
	  g.BindBuffer(GL_ARRAY_BUFFER, matidx);
	  g.VertexAttribPointer(attrMaterial, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	  g.EnableVertexAttribArray(attrMaterial);
	}
	if (attrNormal >= 0) {
	  g.BindBuffer(GL_ARRAY_BUFFER, normalidx);
	  g.VertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	  g.EnableVertexAttribArray(attrNormal);
	}
	if (attrPos >= 0){
	  g.BindBuffer(GL_ARRAY_BUFFER, vtxidx);
	  g.VertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	  g.EnableVertexAttribArray(attrPos);
	}
	g.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexidx);
}

/**
 * ポイントバッファのバインド
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param vtx_radius 半径バッファID
 * @param vtx_material マテリアルバッファID
 */
void BindPointVB_GL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material)
{
	VX::Graphics& g = GetCurrentGraphics();
	GLint attrRadius   = g.GetAttribLocation(prg, "lsgl_PointSize");
	GLint attrMaterial = g.GetAttribLocation(prg, "materialID");
	GLint attrPos      = g.GetAttribLocation(prg, "position");
	
	if (attrRadius >= 0) {
	  g.BindBuffer(GL_ARRAY_BUFFER, vtx_radius);
	  g.VertexAttribPointer(attrRadius, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	  g.EnableVertexAttribArray(attrRadius);
	}
	if (attrMaterial >= 0) {
	  g.BindBuffer(GL_ARRAY_BUFFER, vtx_material);
	  g.VertexAttribPointer(attrMaterial, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	  g.EnableVertexAttribArray(attrMaterial);
	}
	if (attrPos >= 0) {
	  g.BindBuffer(GL_ARRAY_BUFFER, vtxidx);
	  g.VertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	  g.EnableVertexAttribArray(attrPos);
	}
//	g.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexidx);
}

/**
 * フレームバッファのバインド
 * @param frame フレームバッファID
 */
void BindFramebuffer_GL(unsigned int frame)
{
	VX::Graphics& g = GetCurrentGraphics();
	if (frame)
		g.BindFramebuffer(GL_FRAMEBUFFER, frame);
}
           
/**
 * テクスチャバッファの生成
 * @param n 個数
 * @param [out] tex テクスチャバッファインデックス
 */
void GenTextures_GL(int n, unsigned int* tex)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.GenTextures(n, tex);
}

/**
 * 3Dテクスチャのバインド
 * @param texid テクスチャID
 */
void BindTexture3D_GL(unsigned int texid)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.BindTexture(GL_TEXTURE_3D, texid);
}

/**
 * 2Dテクスチャのバインド
 * @param texid テクスチャID
 */
void BindTexture2D_GL(unsigned int texid)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.BindTexture(GL_TEXTURE_2D, texid);
}

/**
 * テクスチャの有効化
 * @param n テクスチャID
 */
void ActiveTexture_GL(unsigned int n)
{
	VX::Graphics& g = GetCurrentGraphics();
	g.ActiveTexture(GL_TEXTURE0 + n);
}

/**
 * テクスチャの生成
 * @param width 幅
 * @param height 高さ
 * @param component 種類
 * @param pixeldata ピクセルデータ
 */
void TexImage2D_GL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata, bool filter)
{
	VX::Graphics& g = GetCurrentGraphics();
	GLint format = GL_LUMINANCE;
	if      (component == 3) format = GL_RGB;
	else if (component == 4) format = GL_RGBA;
	else if (component == 1) format = GL_LUMINANCE;
	else {
		assert(0);
	}
    if (filter) {
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
	g.TexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixeldata);
}

/**
 * Float テクスチャの生成
 * @param width 幅
 * @param height 高さ
 * @param component 種類
 * @param pixeldata ピクセルデータ
 */
void TexImage2DFloat_GL(unsigned int width, unsigned int height, unsigned int component, const float* pixeldata, bool filter)
{
	VX::Graphics& g = GetCurrentGraphics();
	GLint format = GL_LUMINANCE;
	if      (component == 3) format = GL_RGB;
	else if (component == 4) format = GL_RGBA;
	else if (component == 1) format = GL_LUMINANCE;
	else {
		assert(0);
	}
    if (filter) {
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        g.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
	g.TexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, pixeldata);
}


#endif // USE_OPENGL
