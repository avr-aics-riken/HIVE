//
//  Commands_LSGL.cpp
//  AnimTool
//
//  Created by kioku on 13/02/08.
//

/**
 * @file Commands_LSGL.cpp
 * GLコマンド
 */
#include "Commands.h"

#include <GLES2/gl2.h>
#include <gles_context.h>
#include "../Core/vxmath.h"

/**
 * SGLバッファの作成
 * @param w 幅
 * @param h 高さ
 * @param framebuffer フレームバッファ
 * @param colorRenderbuffer カラーレンダーバッファ
 * @param colorbit 色ビット
 * @param despthRenderBuffer デプスレンダーバッファ
 * @param depthbit 深度ビット
 */
void CreateBuffer_SGL(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer, int colorbit, unsigned int& depthRenderbuffer, int depthbit)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenFramebuffers(1, &framebuffer);
	sgl.glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	// create color renderbuffer and attach
	sgl.glGenRenderbuffers(1, &colorRenderbuffer);
	sgl.glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    if (colorbit == 8) {
        sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, w, h);
    } else if (colorbit == 32) {
        sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F_EXT, w, h);
    } else {
        sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, w, h);
        fprintf(stderr, "Error: Invalid color buffer format [colorbit = %d]\n use 8 or 32\n", colorbit);
    }
	sgl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	
	// create depth renderbuffer and attach
	sgl.glGenRenderbuffers(1, &depthRenderbuffer);
	sgl.glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    if (depthbit == 16) {
        sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    } else if (depthbit == 32) {
        sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32_OES, w, h);
    } else {
        sgl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32_OES, w, h);
        fprintf(stderr, "Error: Invalid depth buffer format [depthbit = %d]\n use 16 or 32\n", depthbit);
    }
	sgl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
	sgl.glViewport(0, 0, w, h);
}

/**
 * SGLバッファの解放
 * @param framebuffer フレームバッファ
 * @param colorRenderbuffer カラーレンダーバッファ
 * @param depthRenderbuffer 深度レンダーバッファ
 */
void ReleaseBuffer_SGL(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDeleteRenderbuffers(1, &colorRenderbuffer);
	sgl.glDeleteRenderbuffers(1, &depthRenderbuffer);
	sgl.glDeleteFramebuffers(1, &framebuffer);
}

/**
 * SGLクリア
 * @param red 赤
 * @param green 緑
 * @param blue 青
 * @param alpha 透明度
 */
void Clear_SGL(float red, float green, float blue, float alpha)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glClearDepthf(10000.0);
	sgl.glClearColor(red, green, blue, alpha);
	sgl.glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

/**
 * SGLピクセル値取得
 * @param w 幅
 * @param h 高さ
 * @param imgbuf 結果を格納するバッファ
 * @param colorbit 色ビット
 */
void GetColorBuffer_SGL(int w, int h, unsigned char* imgbuf, int colorbit)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glFinish();
    if (colorbit == 8) {
        sgl.glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, imgbuf);
    } else if (colorbit == 32) {
        sgl.glReadPixels(0, 0, w, h, GL_RGBA, GL_FLOAT, imgbuf);
    } else {
        fprintf(stderr, "Error: Invalid color buffer format [colorbit = %d]\n use 8 or 32\n", colorbit);
    }
}

/**
 * SGLデプスバッファ値取得.
 * @param w 幅
 * @param h 高さ
 * @param depthbuf 結果を格納するバッファ
 */

void GetDepthBuffer_SGL(int w, int h, float* depthbuf)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depthbuf);
}


void CreateFloatBuffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id) {
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glGenBuffers(1, &buf_id);
    sgl.glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num, buffer, GL_STATIC_DRAW);
}
void CreateUintBuffer_SGL(unsigned int num, unsigned int* buffer, unsigned int& buf_id) {
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glGenBuffers(1, &buf_id);
    sgl.glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * num, buffer, GL_STATIC_DRAW);
}
void CreateVec4Buffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id) {
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glGenBuffers(1, &buf_id);
    sgl.glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * num, buffer, GL_STATIC_DRAW);
}
void CreateVec3Buffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id) {
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glGenBuffers(1, &buf_id);
    sgl.glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num, buffer, GL_STATIC_DRAW);
}
void CreateVec2Buffer_SGL(unsigned int num, float* buffer, unsigned int& buf_id) {
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glGenBuffers(1, &buf_id);
    sgl.glBindBuffer(GL_ARRAY_BUFFER, buf_id);
    sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * num, buffer, GL_STATIC_DRAW);
}


/**
 * SGLバッファの作成
 * @param vertexnum 頂点数
 * @param posbuffer 頂点バッファ
 * @param normalbuffer 法線バッファ
 * @param materialbuffer マテリアルバッファ
 * @param texbuffer テクスチャバッファ
 * @param indexnum インデックス数
 * @param indexbuffer インデックスバッファ
 * @param vtx_id 頂点ID
 * @param normal_id 法線ID
 * @param mat_id マテリアルID
 * @param tex_id テクスチャID
 * @param index_id インデックスID
 */
void CreateVBIB_SGL(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* materialbuffer, float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
					unsigned int& vtx_id, unsigned int& normal_id, unsigned int& material_id, unsigned int& tex_id, unsigned int& index_id) {
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenBuffers(1, &vtx_id);
	sgl.glBindBuffer(GL_ARRAY_BUFFER, vtx_id);
	sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, posbuffer, GL_STATIC_DRAW);
    if (normalbuffer) {
        sgl.glGenBuffers(1, &normal_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, normal_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexnum, normalbuffer, GL_STATIC_DRAW);
    }
    if (materialbuffer) {
        sgl.glGenBuffers(1, &material_id);
        sgl.glBindBuffer(GL_ARRAY_BUFFER, material_id);
        sgl.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexnum, materialbuffer, GL_STATIC_DRAW);
    }
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

/**
 * SGLバッファの作成
 * @param vertexnum 頂点数
 * @param posbuffer 頂点バッファ
 * @param radiusbuffer 半径バッファ
 * @param matbuffer マテリアルバッファ
 * @param vtx_id 頂点ID
 * @param radius_id 半径ID
 * @param mat_id マテリアルID
 */
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

/**
 * SGLバッファの作成
 * @param vertexnum 頂点数
 * @param posbuffer 頂点バッファ
 * @param radiusbuffer 半径バッファ
 * @param matbuffer マテリアルバッファ
 * @param indexnum インデックス数
 * @param indexbuffer インデックスバッファ
 * @param vtx_id 頂点ID
 * @param radius_id 半径ID
 * @param mat_id マテリアルID
 * @param index_id インデックスID
 */
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
    sgl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexnum, indexbuffer, GL_DYNAMIC_DRAW);
}

/**
 * SGLバッファの解放
 * @param buffer_id バッファID
 */
void ReleaseBufferVBIB_SGL(unsigned int buffer_id)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDeleteBuffers(1, &buffer_id);
}

/**
 * シェーダプログラムのバインド
 * @param prg シェーダプログラムID
 */
void BindProgram_SGL(unsigned int prg)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glUseProgram(prg);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform1i_SGL(unsigned int prg, const char* name, int val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform1i(p, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform1f_SGL(unsigned int prg, const char* name, float val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform1f(p, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform2fv_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform2fv(p, 1, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform3fv_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform3fv(p, 1, val);
}

/**
 * Uniform値の設定
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniform4fv_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform4fv(p, 1, val);
}

/**
 * Uniform値の設定(配列)
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 * @param num 個数.
 */
void SetUniformFloatArray_SGL(unsigned int prg, const char* name, const float* val, int num)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniform1fv(p, num, val);
}

/**
 * Uniform値の設定(行列)
 * @param prg シェーダプログラムID
 * @param name 名前
 * @param val 値
 */
void SetUniformMatrix_SGL(unsigned int prg, const char* name, const float* val)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	const int p = sgl.glGetUniformLocation(prg, name);
	if (p != -1)
		sgl.glUniformMatrix4fv(p, 1, 0, val);
}

/**
 * SGLカメラの設定
 * @param prg シェーダプログラムID
 * @param eye 視点
 * @param lookat 注目点
 * @param up アップベクトル
 * @param fov 視野角
 */
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

/**
 * SGLステレオカメラの設定
 * @param prg シェーダプログラムID
 * @param eye 視点
 * @param lookat 注目点
 * @param up アップベクトル
 * @param zeroParallax ゼロ視差
 * @param eyeSeparation 両目間隔
 */
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

/// インデックスで描画
/// @param indexnum インデックス数
void DrawElements_SGL(unsigned int indexnum)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);
}

/// インデックスでライン描画
/// @param indexnum インデックス数
void DrawLineElements_SGL(unsigned int indexnum)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glDrawElements(GL_LINES, indexnum, GL_UNSIGNED_INT, (void*)0);
}

/// 三角形の描画.
/// @param vtxnum 頂点数.
void DrawArrays_SGL(unsigned int vtxnum)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDrawArrays(GL_TRIANGLES, 0, vtxnum);
}

/// 点の描画.
/// @param vtxnum 頂点数.
void DrawPointArrays_SGL(unsigned int vtxnum)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDrawArrays(GL_POINTS, 0, vtxnum);
}

/// ラインの描画.
/// @param vtxnum 頂点数.
void DrawLineArrays_SGL(unsigned int vtxnum)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glDrawArrays(GL_LINES, 0, vtxnum);
}

/// 三角錐の描画.
/// @param vtxnum 頂点数.
void DrawTetraArrays_SGL(unsigned int vtxnum)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glDrawArrays(GL_TETRAHEDRONS_EXT, 0, vtxnum);
}

/// Solid の描画.
/// @param vtxnum 頂点数.
void DrawSolidArrays_SGL(int solidType, unsigned int vtxnum)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
		if (solidType == 5) {
			sgl.glDrawArrays(GL_PYRAMIDS_EXT, 0, vtxnum);
		} else if (solidType == 6) {
			sgl.glDrawArrays(GL_PRISMS_EXT, 0, vtxnum);
		} else if (solidType == 8) {
			sgl.glDrawArrays(GL_HEXAHEDRONS_EXT, 0, vtxnum);
		}
}

/**
 * 遮蔽率の指定.
 * @param a 遮蔽率.
 * @param invert 遮蔽率マスクの反転フラグ.
 */
void SampleCoverage_SGL(float a, bool invert)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glEnable(GL_SAMPLE_COVERAGE);
	sgl.glSampleCoverage(a, invert);
}

/**
 * ピクセルステップの指定.
 * @param n ピクセルステップ
 */
void PixelStep_SGL(int n)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglSetPixelStep(n);
}

/**
 * シェーダコンパイラの設定.
 * @param path シェーダコンパイラのパス.
 * @param opt オプション.
 */
void SetShaderCompiler_SGL(const char* path, const char* opt)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.lsglSetShaderCompiler(path, opt);
}

/**
 * 並列レンダリングの設定.
 * @param enable 並列レンダリング有効フラグ
 * @param mergeEnable 並列レンダリング有効フラグ
 */
void SetScreenParallel_SGL(bool enable, bool mergeEnable)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.lsglSetScreenParallelRendering(enable, mergeEnable);
}

/**
 * 文字列バッファ.
 */
class stringbuf
{
public:
    stringbuf() : buf(0) {}
    ~stringbuf() { delete [] buf;}
    void resize(int n) {
        delete [] buf;
        buf = new char[n];
    }
    char* get() const { return buf; }
private:
    char* buf;
};

namespace {

    void printShaderInfoLog(GLuint shader)
    {
        static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
        int bufSize = 0;
        sgl.glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
        if (bufSize > 1) {
            char* infoLog;
            infoLog = new char[bufSize];
            if (infoLog != NULL) {
                int length;
                glGetShaderInfoLog(shader, bufSize, &length, infoLog);
                fprintf(stderr, "InfoLog:\n%s\n", infoLog);
                delete [] infoLog;
            } else {
                fprintf(stderr, "Could not allocate InfoLog buffer.");
            }
        }
    }

    void printProgramInfoLog(GLuint program)
    {
        static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
        int bufSize;
        sgl.glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
        if (bufSize > 1) {
            char *infoLog;
            infoLog = new char[bufSize];
            if (infoLog != NULL) {
                int length;
                glGetProgramInfoLog(program, bufSize, &length, infoLog);
                fprintf(stderr, "InfoLog:\n%s\n", infoLog);
                delete [] infoLog;
            } else {
                fprintf(stderr, "Could not allocate InfoLog buffer.");
            }
        }
    }

}


/**
 * シェーダプログラムの作成.
 * @param srcname ソースファイルパス.
 * @param [out] prg シェーダプログラムID
 */
bool CreateProgramSrc_SGL(const char* srcname, unsigned int& prg)
{
    printf("CreateProgramSrc\n");

	//static GLchar srcbuf[16384];
    stringbuf srcbuf;
	FILE *fp = fopen(srcname, "rb");
	if (!fp){
		fprintf(stderr, "Error: Not found Program [%s]\n", srcname);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
    srcbuf.resize(len + 2);
	rewind(fp);
	len = fread(srcbuf.get(), 1, len, fp);
    srcbuf.get()[len    ] = 0; // terminate
    srcbuf.get()[len + 1] = 0;
	fclose(fp);
	
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	if (prg != 0)
		sgl.glDeleteProgram(prg);
	prg = 0;
	
    const GLchar *src = srcbuf.get();
	unsigned int fragShader;
	fragShader = sgl.glCreateShader(GL_FRAGMENT_SHADER);
	sgl.glShaderSource(fragShader, 1, &src, NULL);
	sgl.glCompileShader(fragShader);
	GLint val = 0;
	sgl.glGetShaderiv(fragShader, GL_COMPILE_STATUS, &val);
	//assert(val == GL_TRUE && "failed to compile shader");
	if (val!=GL_TRUE) {
		fprintf(stderr, "Error: failed to compile shader [%s]\n", srcname);
        printShaderInfoLog(fragShader);
		return false;
	}
	
	prg = sgl.glCreateProgram();
	sgl.glAttachShader(prg, fragShader);
	sgl.glLinkProgram(prg);
	sgl.glGetProgramiv(prg, GL_LINK_STATUS, &val);
	//assert(val == GL_TRUE && "failed to link shader");
	if (val!=GL_TRUE) {
		fprintf(stderr, "Error: failed to link shader [%s]\n", srcname);
		return false;
	}
	
	return true;
}

/**
 * バイナリシェーダプログラムの作成.
 * @param soname ソースファイルパス.
 * @param [out] prg シェーダプログラムID.
 */
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

/**
 * シェーダプログラムの削除.
 * @param prg シェーダプログラムID.
 */
bool DeleteProgram_SGL(unsigned int prg)
{
	assert(prg);
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glDeleteProgram(prg);
	return true;
}

void BindBufferFloat_SGL(unsigned int prg, const char* attrname, unsigned int bufidx)
{
    assert(prg);
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attr = sgl.glGetAttribLocation(prg, attrname);
    if (attr != -1 && bufidx != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, bufidx);
        sgl.glVertexAttribPointer(attr, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        sgl.glEnableVertexAttribArray(attr);
    }
}
void BindBufferUint_SGL(unsigned int prg, const char* attrname, unsigned int bufidx)
{
    assert(prg);
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attr = sgl.glGetAttribLocation(prg, attrname);
    if (attr != -1 && bufidx != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, bufidx);
        sgl.glVertexAttribPointer(attr, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(unsigned int), (void*)0);
        sgl.glEnableVertexAttribArray(attr);
    }
}
void BindBufferVec4_SGL(unsigned int prg, const char* attrname, unsigned int bufidx)
{
    assert(prg);
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attr = sgl.glGetAttribLocation(prg, attrname);
    if (attr != -1 && bufidx != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, bufidx);
        sgl.glVertexAttribPointer(attr, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
        sgl.glEnableVertexAttribArray(attr);
    }
}
void BindBufferVec3_SGL(unsigned int prg, const char* attrname, unsigned int bufidx)
{
    assert(prg);
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attr = sgl.glGetAttribLocation(prg, attrname);
    if (attr != -1 && bufidx != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, bufidx);
        sgl.glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
        sgl.glEnableVertexAttribArray(attr);
    }
}
void BindBufferVec2_SGL(unsigned int prg, const char* attrname, unsigned int bufidx)
{
    assert(prg);
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attr = sgl.glGetAttribLocation(prg, attrname);
    if (attr != -1 && bufidx != -1) {
        sgl.glBindBuffer(GL_ARRAY_BUFFER, bufidx);
        sgl.glVertexAttribPointer(attr, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
        sgl.glEnableVertexAttribArray(attr);
    }
}


/**
 * バッファのバインド.
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param normalidx 法線インデックスバッファID
 * @param vtx_material マテリアルインデックスバッファID
 * @param texidx テクスチャインデックスバッファID
 * @param indexidx インデックスバッファID
 */
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

/**
 * ポイントバッファのバインド
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param vtx_radius 半径バッファID
 * @param vtx_material マテリアルバッファID
 */
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

/**
 * ラインバッファのバインド
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param vtx_radius 半径バッファID
 * @param vtx_material マテリアルバッファID
 * @param indexidx インデックスバッファID
 */
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

/**
 * 三角錐バッファのバインド
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param vtx_radius 半径バッファID
 * @param vtx_material マテリアルバッファID
 * @param indexidx インデックスバッファID
 */
void BindTetraVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attrMaterial = sgl.glGetAttribLocation(prg, "matID");
    GLint attrPos      = sgl.glGetAttribLocation(prg, "position");
    
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

/**
 * Solid バッファのバインド
 * @param prg シェーダプログラムID
 * @param vtxidx 頂点インデックスバッファID
 * @param vtx_radius 半径バッファID
 * @param vtx_material マテリアルバッファID
 * @param indexidx インデックスバッファID
 */
void BindSolidVBIB_SGL(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    GLint attrMaterial = sgl.glGetAttribLocation(prg, "matID");
    GLint attrPos      = sgl.glGetAttribLocation(prg, "position");
    
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

/**
 * ライン幅の指定.
 * @param w ライン幅.
 */
void LineWidth_SGL(float w)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glLineWidth(w);
}

/**
 * テクスチャの生成.
 * @param n 個数.
 * @param [out] テクスチャID.
 */
void GenTextures_SGL(int n, unsigned int* tex)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glGenTextures(n, tex);
}
/**
 * テクスチャの削除.
 * @param n 個数
 * @param tex テクスチャID
 */
void DeleteTextures_SGL(int n, unsigned int* tex)
{
    static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
    sgl.glDeleteTextures(n, tex);
}

/**
 * 3Dテクスチャのバインド
 * @param texid テクスチャID
 */
void BindTexture3D_SGL(unsigned int voltex)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glBindTexture(GL_TEXTURE_3D, voltex);
}

/**
 * 2Dテクスチャのバインド
 * @param texid テクスチャID
 */
void BindTexture2D_SGL(unsigned int texid)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glBindTexture(GL_TEXTURE_2D, texid);
}

/**
 * テクスチャの有効化
 * @param n テクスチャID
 */
void ActiveTexture_SGL(unsigned int n)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glActiveTexture(GL_TEXTURE0 + n);
}

/**
 * テクスチャの生成
 * @param width 幅
 * @param height 高さ
 * @param component 種類
 * @param pixeldata ピクセルデータ
 */
void TexImage2D_SGL(unsigned int width, unsigned int height, unsigned int component, const unsigned char* pixeldata, bool filter, bool clampToEdgeS, bool clampToEdgeT)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	GLint format = GL_LUMINANCE;
	if      (component == 3) format = GL_RGB;
	else if (component == 4) format = GL_RGBA;
	else if (component == 1) format = GL_LUMINANCE;
	else {
		assert(0);
	}
    sgl.glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixeldata);

    if (filter) {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (clampToEdgeS) {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    if (clampToEdgeT) {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

}

/**
 * Float テクスチャの生成
 * @param width 幅
 * @param height 高さ
 * @param component 種類
 * @param pixeldata ピクセルデータ
 */
void TexImage2DFloat_SGL(unsigned int width, unsigned int height, unsigned int component, const float* pixeldata, bool filter, bool clampToEdgeS, bool clampToEdgeT)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	GLint format = GL_LUMINANCE;
	if      (component == 3) format = GL_RGB;
	else if (component == 4) format = GL_RGBA;
	else if (component == 1) format = GL_LUMINANCE;
	else {
		assert(0);
	}
    sgl.glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, pixeldata);
    if (filter) {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (clampToEdgeS) {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    if (clampToEdgeT) {
        sgl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

/**
 * 3Dテクスチャの生成
 * @param width 幅
 * @param height 高さ
 * @param depth 深度
 * @param component 種類
 * @param volumedata ボリュームデータ
 */
void TexImage3DPointer_SGL(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR)
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

    if (clampToEdgeS) {
        sgl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    if (clampToEdgeT) {
        sgl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    if (clampToEdgeR) {
        sgl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

/**
 * Sparse 3Dテクスチャの生成
 * @param level LoD level
 * @param width 空間上での幅
 * @param height 空間上での高さ
 * @param depth 空間上での深度
 * @param cellWidth cell 幅
 * @param cellHeight cell 高さ
 * @param cellDepth cell 深度
 * @param component 種類
 * @param volumedata ボリュームデータ
 */
void SparseTexImage3DPointer_SGL(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int depth, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR)
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
	sgl.lsglTexPageCommitment(GL_TEXTURE_3D, 0, xoffset, yoffset, zoffset, width, height, depth, GL_TRUE);
	sgl.lsglTexSubImage3DPointer(GL_TEXTURE_3D, level, xoffset, yoffset, zoffset, width, height, depth, cellWidth, cellHeight, cellDepth, format, GL_FLOAT, volumedata);

	// @fixme { Setting TexParameter is only required to set once, not everytime. }
    if (clampToEdgeS) {
        sgl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    if (clampToEdgeT) {
        sgl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    if (clampToEdgeR) {
        sgl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

void TexCoordRemap3D_SGL(int axis, int n, const float* values)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	if (axis == 0) {
		sgl.lsglTexCoordRemap(GL_TEXTURE_3D, GL_COORDINATE_X, n, values);
	} else if (axis == 1) {
		sgl.lsglTexCoordRemap(GL_TEXTURE_3D, GL_COORDINATE_Y, n, values);
	} else if (axis == 2) {
		sgl.lsglTexCoordRemap(GL_TEXTURE_3D, GL_COORDINATE_Z, n, values);
	}
}

/**
 * SGLコマンドバッファの完了.
 */
void Finish_SGL()
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.glFinish();
}

/**
 * SGLフラグメントシェーダの評価.
 */
void EvalFragmentShader_SGL()
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglEvalFragmentShader();
}

/**
 * コールバックの設定
 * @param callback コールバック
 * @param userptr ユーザデータポインタ.
 */
void SetCallback_SGL(LSGLProgressCallback callback, void* userptr)
{
	static lsgl::Context& sgl = lsgl::Context::GetCurrentContext();
	sgl.lsglSetProgressCallback(callback, userptr);
}


