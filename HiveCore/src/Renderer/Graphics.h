/**
 * @file Graphics.h
 * 
 * V-Xgen Graphics library
 * coded by Kentaro Oku
 * 
 */

#ifndef INCLUDE_VXGRAPHICS_H
#define INCLUDE_VXGRAPHICS_H

namespace VX {
    
typedef unsigned int VGenum;

// オーバーヘッド回避のため純粋仮想関数にはしない
// プラットフォームごとにinlineでメソッドを定義する
class Graphics
{
public:
	Graphics();
	~Graphics();
	
private:
	void* m_extptr;
	unsigned int m_bind_vertexbuffer;
	unsigned int m_bind_indexbuffer;
	unsigned int m_bind_texture;
	
public:
	// OpenGLES 2.0 相当Functions
	void ActiveTexture (VGenum texture);
	void AttachShader (unsigned int program, unsigned int shader);
	void BindAttribLocation (unsigned int program, unsigned int index, const char* name);
	void BindBuffer (VGenum target, unsigned int buffer);
	void BindFramebuffer (VGenum target, unsigned int framebuffer);
	void BindRenderbuffer (VGenum target, unsigned int renderbuffer);
	void BindTexture (VGenum target, unsigned int texture);
	void BlendColor (float red, float green, float blue, float alpha);
	void BlendEquation ( VGenum mode );
	void BlendEquationSeparate (VGenum modeRGB, VGenum modeAlpha);
	void BlendFunc (VGenum sfactor, VGenum dfactor);
	void BlendFuncSeparate (VGenum srcRGB, VGenum dstRGB, VGenum srcAlpha, VGenum dstAlpha);
	void BufferData (VGenum target, int size, const void* data, VGenum usage);
	void BufferSubData (VGenum target, int offset, int size, const void* data);
	VGenum CheckFramebufferStatus (VGenum target);
	void Clear (unsigned int mask);
	void ClearColor (float red, float green, float blue, float alpha);
	void ClearDepthf (float depth);
	void ClearStencil (int s);
	void ColorMask (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
	void CompileShader (unsigned int shader);
	void CompressedTexImage2D (VGenum target, int level, VGenum internalformat, int width, int height, int border, int imageSize, const void* data);
	void CompressedTexSubImage2D (VGenum target, int level, int xoffset, int yoffset, int width, int height, VGenum format, int imageSize, const void* data);
	void CopyTexImage2D (VGenum target, int level, VGenum internalformat, int x, int y, int width, int height, int border);
	void CopyTexSubImage2D (VGenum target, int level, int xoffset, int yoffset, int x, int y, int width, int height);
	unsigned int  CreateProgram (void);
	unsigned int  CreateShader (VGenum type);
	void CullFace (VGenum mode);
	void DeleteBuffers (int n, const unsigned int* buffers);
	void DeleteFramebuffers (int n, const unsigned int* framebuffers);
	void DeleteProgram (unsigned int program);
	void DeleteRenderbuffers (int n, const unsigned int* renderbuffers);
	void DeleteShader (unsigned int shader);
	void DeleteTextures (int n, const unsigned int* textures);
	void DepthFunc (VGenum func);
	void DepthMask (unsigned char flag);
	void DepthRangef (float zNear, float zFar);
	void DetachShader (unsigned int program, unsigned int shader);
	void Disable (VGenum cap);
	void DisableVertexAttribArray (unsigned int index);
	void DrawArrays (VGenum mode, int first, int count);
	void DrawElements (VGenum mode, int count, VGenum type, const void* indices);
	void Enable (VGenum cap);
	void EnableVertexAttribArray (unsigned int index);
	void Finish (void);
	void Flush (void);
	void FramebufferRenderbuffer (VGenum target, VGenum attachment, VGenum renderbuffertarget, unsigned int renderbuffer);
	void FramebufferTexture2D (VGenum target, VGenum attachment, VGenum textarget, unsigned int texture, int level);
	void FrontFace (VGenum mode);
	void GenBuffers (int n, unsigned int* buffers);
	void GenerateMipmap (VGenum target);
	void GenFramebuffers (int n, unsigned int* framebuffers);
	void GenRenderbuffers (int n, unsigned int* renderbuffers);
	void GenTextures (int n, unsigned int* textures);
	void GetActiveAttrib (unsigned int program, unsigned int index, int bufsize, int* length, int* size, VGenum* type, char* name);
	void GetActiveUniform (unsigned int program, unsigned int index, int bufsize, int* length, int* size, VGenum* type, char* name);
	void GetAttachedShaders (unsigned int program, int maxcount, int* count, unsigned int* shaders);
	int  GetAttribLocation (unsigned int program, const char* name);
	void GetBooleanv (VGenum pname, unsigned char* params);
	void GetBufferParameteriv (VGenum target, VGenum pname, int* params);
	VGenum GetError (void);
	void GetFloatv (VGenum pname, float* params);
	void GetFramebufferAttachmentParameteriv (VGenum target, VGenum attachment, VGenum pname, int* params);
	void GetIntegerv (VGenum pname, int* params);
	void GetProgramiv (unsigned int program, VGenum pname, int* params);
	void GetProgramInfoLog (unsigned int program, int bufsize, int* length, char* infolog);
	void GetRenderbufferParameteriv (VGenum target, VGenum pname, int* params);
	void GetShaderiv (unsigned int shader, VGenum pname, int* params);
	void GetShaderInfoLog (unsigned int shader, int bufsize, int* length, char* infolog);
	void GetShaderPrecisionFormat (VGenum shadertype, VGenum precisiontype, int* range, int* precision);
	void GetShaderSource (unsigned int shader, int bufsize, int* length, char* source);
	const unsigned char* GetString (VGenum name);
	void GetTexParameterfv (VGenum target, VGenum pname, float* params);
	void GetTexParameteriv (VGenum target, VGenum pname, int* params);
	void GetUniformfv (unsigned int program, int location, float* params);
	void GetUniformiv (unsigned int program, int location, int* params);
	int  GetUniformLocation (unsigned int program, const char* name);
	void GetVertexAttribfv (unsigned int index, VGenum pname, float* params);
	void GetVertexAttribiv (unsigned int index, VGenum pname, int* params);
	void GetVertexAttribPointerv (unsigned int index, VGenum pname, void** pointer);
	void Hint (VGenum target, VGenum mode);
	unsigned char   IsBuffer (unsigned int buffer);
	unsigned char   IsEnabled (VGenum cap);
	unsigned char   IsFramebuffer (unsigned int framebuffer);
	unsigned char   IsProgram (unsigned int program);
	unsigned char   IsRenderbuffer (unsigned int renderbuffer);
	unsigned char   IsShader (unsigned int shader);
	unsigned char   IsTexture (unsigned int texture);
	void LineWidth (float width);
	void LinkProgram (unsigned int program);
	void PixelStorei (VGenum pname, int param);
	void PolygonOffset (float factor, float units);
	void ReadPixels (int x, int y, int width, int height, VGenum format, VGenum type, void* pixels);
	void ReleaseShaderCompiler (void);
	void RenderbufferStorage (VGenum target, VGenum internalformat, int width, int height);
	void SampleCoverage (float value, unsigned char invert);
	void Scissor (int x, int y, int width, int height);
	void ShaderBinary (int n, const unsigned int* shaders, VGenum binaryformat, const void* binary, int length);
	void ShaderSource (unsigned int shader, int count, const char** string, const int* length);
	void StencilFunc (VGenum func, int ref, unsigned int mask);
	void StencilFuncSeparate (VGenum face, VGenum func, int ref, unsigned int mask);
	void StencilMask (unsigned int mask);
	void StencilMaskSeparate (VGenum face, unsigned int mask);
	void StencilOp (VGenum fail, VGenum zfail, VGenum zpass);
	void StencilOpSeparate (VGenum face, VGenum fail, VGenum zfail, VGenum zpass);
	void TexImage2D (VGenum target, int level, int internalformat, int width, int height, int border, VGenum format, VGenum type, const void* pixels);
	void TexParameterf  (VGenum target, VGenum pname, float param);
	void TexParameterfv (VGenum target, VGenum pname, const float* params);
	void TexParameteri  (VGenum target, VGenum pname, int param);
	void TexParameteriv (VGenum target, VGenum pname, const int* params);
	void TexSubImage2D  (VGenum target, int level, int xoffset, int yoffset, int width, int height, VGenum format, VGenum type, const void* pixels);
	void Uniform1f  (int location, float x);
	void Uniform1fv (int location, int count, const float* v);
	void Uniform1i  (int location, int x);
	void Uniform1iv (int location, int count, const int* v);
	void Uniform2f  (int location, float x, float y);
	void Uniform2fv (int location, int count, const float* v);
	void Uniform2i  (int location, int x, int y);
	void Uniform2iv (int location, int count, const int* v);
	void Uniform3f  (int location, float x, float y, float z);
	void Uniform3fv (int location, int count, const float* v);
	void Uniform3i  (int location, int x, int y, int z);
	void Uniform3iv (int location, int count, const int* v);
	void Uniform4f  (int location, float x, float y, float z, float w);
	void Uniform4fv (int location, int count, const float* v);
	void Uniform4i  (int location, int x, int y, int z, int w);
	void Uniform4iv (int location, int count, const int* v);
	void UniformMatrix2fv (int location, int count, unsigned char transpose, const float* value);
	void UniformMatrix3fv (int location, int count, unsigned char transpose, const float* value);
	void UniformMatrix4fv (int location, int count, unsigned char transpose, const float* value);
	void UseProgram (unsigned int program);
	void ValidateProgram (unsigned int program);
	void VertexAttrib1f  (unsigned int indx, float x);
	void VertexAttrib1fv (unsigned int indx, const float* values);
	void VertexAttrib2f  (unsigned int indx, float x, float y);
	void VertexAttrib2fv (unsigned int indx, const float* values);
	void VertexAttrib3f  (unsigned int indx, float x, float y, float z);
	void VertexAttrib3fv (unsigned int indx, const float* values);
	void VertexAttrib4f  (unsigned int indx, float x, float y, float z, float w);
	void VertexAttrib4fv (unsigned int indx, const float* values);
	void VertexAttribPointer (unsigned int indx, int size, VGenum type, unsigned char normalized, int stride, const void* ptr);
	void Viewport (int x, int y, int width, int height);
	
	// Extention
	void PolygonMode (VGenum face, VGenum mode);
};

} // namespace VX

/* ClearBufferMask */
#define VG_DEPTH_BUFFER_BIT               0x00000100
#define VG_STENCIL_BUFFER_BIT             0x00000400
#define VG_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define VG_FALSE                          0
#define VG_TRUE                           1

/* BeginMode */
#define VG_POINTS                         0x0000
#define VG_LINES                          0x0001
#define VG_LINE_LOOP                      0x0002
#define VG_LINE_STRIP                     0x0003
#define VG_TRIANGLES                      0x0004
#define VG_TRIANGLE_STRIP                 0x0005
#define VG_TRIANGLE_FAN                   0x0006

/* AlphaFunction (not supported in ES20) */
/*      VG_NEVER */
/*      VG_LESS */
/*      VG_EQUAL */
/*      VG_LEQUAL */
/*      VG_GREATER */
/*      VG_NOTEQUAL */
/*      VG_GEQUAL */
/*      VG_ALWAYS */

/* BlendingFactorDest */
#define VG_ZERO                           0
#define VG_ONE                            1
#define VG_SRC_COLOR                      0x0300
#define VG_ONE_MINUS_SRC_COLOR            0x0301
#define VG_SRC_ALPHA                      0x0302
#define VG_ONE_MINUS_SRC_ALPHA            0x0303
#define VG_DST_ALPHA                      0x0304
#define VG_ONE_MINUS_DST_ALPHA            0x0305

/* BlendingFactorSrc */
/*      VG_ZERO */
/*      VG_ONE */
#define VG_DST_COLOR                      0x0306
#define VG_ONE_MINUS_DST_COLOR            0x0307
#define VG_SRC_ALPHA_SATURATE             0x0308
/*      VG_SRC_ALPHA */
/*      VG_ONE_MINUS_SRC_ALPHA */
/*      VG_DST_ALPHA */
/*      VG_ONE_MINUS_DST_ALPHA */

/* BlendEquationSeparate */
#define VG_FUNC_ADD                       0x8006
#define VG_BLEND_EQUATION                 0x8009
#define VG_BLEND_EQUATION_RGB             0x8009    /* same as BLEND_EQUATION */
#define VG_BLEND_EQUATION_ALPHA           0x883D

/* BlendSubtract */
#define VG_FUNC_SUBTRACT                  0x800A
#define VG_FUNC_REVERSE_SUBTRACT          0x800B

/* Separate Blend Functions */
#define VG_BLEND_DST_RGB                  0x80C8
#define VG_BLEND_SRC_RGB                  0x80C9
#define VG_BLEND_DST_ALPHA                0x80CA
#define VG_BLEND_SRC_ALPHA                0x80CB
#define VG_CONSTANT_COLOR                 0x8001
#define VG_ONE_MINUS_CONSTANT_COLOR       0x8002
#define VG_CONSTANT_ALPHA                 0x8003
#define VG_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define VG_BLEND_COLOR                    0x8005

/* Buffer Objects */
#define VG_ARRAY_BUFFER                   0x8892
#define VG_ELEMENT_ARRAY_BUFFER           0x8893
#define VG_ARRAY_BUFFER_BINDING           0x8894
#define VG_ELEMENT_ARRAY_BUFFER_BINDING   0x8895

#define VG_STREAM_DRAW                    0x88E0
#define VG_STATIC_DRAW                    0x88E4
#define VG_DYNAMIC_DRAW                   0x88E8

#define VG_BUFFER_SIZE                    0x8764
#define VG_BUFFER_USAGE                   0x8765

#define VG_CURRENT_VERTEX_ATTRIB          0x8626

/* CullFaceMode */
#define VG_FRONT                          0x0404
#define VG_BACK                           0x0405
#define VG_FRONT_AND_BACK                 0x0408

/* PolygonMode */
#define VG_POINT                          0x1B00
#define VG_LINE                           0x1B01
#define VG_FILL                           0x1B02

/* DepthFunction */
/*      VG_NEVER */
/*      VG_LESS */
/*      VG_EQUAL */
/*      VG_LEQUAL */
/*      VG_GREATER */
/*      VG_NOTEQUAL */
/*      VG_GEQUAL */
/*      VG_ALWAYS */

/* EnableCap */
#define VG_TEXTURE_2D                     0x0DE1
#define VG_CULL_FACE                      0x0B44
#define VG_BLEND                          0x0BE2
#define VG_DITHER                         0x0BD0
#define VG_STENCIL_TEST                   0x0B90
#define VG_DEPTH_TEST                     0x0B71
#define VG_SCISSOR_TEST                   0x0C11
#define VG_POLYGON_OFFSET_FILL            0x8037
#define VG_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define VG_SAMPLE_COVERAGE                0x80A0

/* ErrorCode */
#define VG_NO_ERROR                       0
#define VG_INVALID_ENUM                   0x0500
#define VG_INVALID_VALUE                  0x0501
#define VG_INVALID_OPERATION              0x0502
#define VG_OUT_OF_MEMORY                  0x0505

/* FrontFaceDirection */
#define VG_CW                             0x0900
#define VG_CCW                            0x0901

/* GetPName */
#define VG_LINE_WIDTH                     0x0B21
#define VG_ALIASED_POINT_SIZE_RANGE       0x846D
#define VG_ALIASED_LINE_WIDTH_RANGE       0x846E
#define VG_CULL_FACE_MODE                 0x0B45
#define VG_FRONT_FACE                     0x0B46
#define VG_DEPTH_RANGE                    0x0B70
#define VG_DEPTH_WRITEMASK                0x0B72
#define VG_DEPTH_CLEAR_VALUE              0x0B73
#define VG_DEPTH_FUNC                     0x0B74
#define VG_STENCIL_CLEAR_VALUE            0x0B91
#define VG_STENCIL_FUNC                   0x0B92
#define VG_STENCIL_FAIL                   0x0B94
#define VG_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define VG_STENCIL_PASS_DEPTH_PASS        0x0B96
#define VG_STENCIL_REF                    0x0B97
#define VG_STENCIL_VALUE_MASK             0x0B93
#define VG_STENCIL_WRITEMASK              0x0B98
#define VG_STENCIL_BACK_FUNC              0x8800
#define VG_STENCIL_BACK_FAIL              0x8801
#define VG_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define VG_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define VG_STENCIL_BACK_REF               0x8CA3
#define VG_STENCIL_BACK_VALUE_MASK        0x8CA4
#define VG_STENCIL_BACK_WRITEMASK         0x8CA5
#define VG_VIEWPORT                       0x0BA2
#define VG_SCISSOR_BOX                    0x0C10
/*      VG_SCISSOR_TEST */
#define VG_COLOR_CLEAR_VALUE              0x0C22
#define VG_COLOR_WRITEMASK                0x0C23
#define VG_UNPACK_ALIGNMENT               0x0CF5
#define VG_PACK_ALIGNMENT                 0x0D05
#define VG_MAX_TEXTURE_SIZE               0x0D33
#define VG_MAX_VIEWPORT_DIMS              0x0D3A
#define VG_SUBPIXEL_BITS                  0x0D50
#define VG_RED_BITS                       0x0D52
#define VG_GREEN_BITS                     0x0D53
#define VG_BLUE_BITS                      0x0D54
#define VG_ALPHA_BITS                     0x0D55
#define VG_DEPTH_BITS                     0x0D56
#define VG_STENCIL_BITS                   0x0D57
#define VG_POLYGON_OFFSET_UNITS           0x2A00
/*      VG_POLYGON_OFFSET_FILL */
#define VG_POLYGON_OFFSET_FACTOR          0x8038
#define VG_TEXTURE_BINDING_2D             0x8069
#define VG_SAMPLE_BUFFERS                 0x80A8
#define VG_SAMPLES                        0x80A9
#define VG_SAMPLE_COVERAGE_VALUE          0x80AA
#define VG_SAMPLE_COVERAGE_INVERT         0x80AB

/* GetTextureParameter */
/*      VG_TEXTURE_MAG_FILTER */
/*      VG_TEXTURE_MIN_FILTER */
/*      VG_TEXTURE_WRAP_S */
/*      VG_TEXTURE_WRAP_T */

#define VG_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define VG_COMPRESSED_TEXTURE_FORMATS     0x86A3

/* HintMode */
#define VG_DONT_CARE                      0x1100
#define VG_FASTEST                        0x1101
#define VG_NICEST                         0x1102

/* HintTarget */
#define VG_GENERATE_MIPMAP_HINT            0x8192

/* DataType */
#define VG_BYTE                           0x1400
#define VG_UNSIGNED_BYTE                  0x1401
#define VG_SHORT                          0x1402
#define VG_UNSIGNED_SHORT                 0x1403
#define VG_INT                            0x1404
#define VG_UNSIGNED_INT                   0x1405
#define VG_FLOAT                          0x1406
#define VG_FIXED                          0x140C

/* PixelFormat */
#define VG_DEPTH_COMPONENT                0x1902
#define VG_ALPHA                          0x1906
#define VG_RGB                            0x1907
#define VG_BGRA                           0x80E1 // will support extension!
#define VG_RGBA                           0x1908
#define VG_LUMINANCE                      0x1909
#define VG_LUMINANCE_ALPHA                0x190A

/* PixelType */
/*      VG_UNSIGNED_BYTE */
#define VG_UNSIGNED_SHORT_4_4_4_4         0x8033
#define VG_UNSIGNED_SHORT_5_5_5_1         0x8034
#define VG_UNSIGNED_SHORT_5_6_5           0x8363

/* Shaders */
#define VG_FRAGMENT_SHADER                0x8B30
#define VG_VERTEX_SHADER                  0x8B31
#define VG_MAX_VERTEX_ATTRIBS             0x8869
#define VG_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define VG_MAX_VARYING_VECTORS            0x8DFC
#define VG_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define VG_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define VG_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define VG_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define VG_SHADER_TYPE                    0x8B4F
#define VG_DELETE_STATUS                  0x8B80
#define VG_LINK_STATUS                    0x8B82
#define VG_VALIDATE_STATUS                0x8B83
#define VG_ATTACHED_SHADERS               0x8B85
#define VG_ACTIVE_UNIFORMS                0x8B86
#define VG_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define VG_ACTIVE_ATTRIBUTES              0x8B89
#define VG_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define VG_SHADING_LANGUAGE_VERSION       0x8B8C
#define VG_CURRENT_PROGRAM                0x8B8D

/* StencilFunction */
#define VG_NEVER                          0x0200
#define VG_LESS                           0x0201
#define VG_EQUAL                          0x0202
#define VG_LEQUAL                         0x0203
#define VG_GREATER                        0x0204
#define VG_NOTEQUAL                       0x0205
#define VG_GEQUAL                         0x0206
#define VG_ALWAYS                         0x0207

/* StencilOp */
/*      VG_ZERO */
#define VG_KEEP                           0x1E00
#define VG_REPLACE                        0x1E01
#define VG_INCR                           0x1E02
#define VG_DECR                           0x1E03
#define VG_INVERT                         0x150A
#define VG_INCR_WRAP                      0x8507
#define VG_DECR_WRAP                      0x8508

/* StringName */
#define VG_VENDOR                         0x1F00
#define VG_RENDERER                       0x1F01
#define VG_VERSION                        0x1F02
#define VG_EXTENSIONS                     0x1F03

/* TextureMagFilter */
#define VG_NEAREST                        0x2600
#define VG_LINEAR                         0x2601

/* TextureMinFilter */
/*      VG_NEAREST */
/*      VG_LINEAR */
#define VG_NEAREST_MIPMAP_NEAREST         0x2700
#define VG_LINEAR_MIPMAP_NEAREST          0x2701
#define VG_NEAREST_MIPMAP_LINEAR          0x2702
#define VG_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define VG_TEXTURE_MAG_FILTER             0x2800
#define VG_TEXTURE_MIN_FILTER             0x2801
#define VG_TEXTURE_WRAP_S                 0x2802
#define VG_TEXTURE_WRAP_T                 0x2803

/* TextureTarget */
/*      VG_TEXTURE_2D */
#define VG_TEXTURE                        0x1702

#define VG_TEXTURE_CUBE_MAP               0x8513
#define VG_TEXTURE_BINDING_CUBE_MAP       0x8514
#define VG_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define VG_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define VG_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define VG_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define VG_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define VG_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define VG_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C

/* TextureUnit */
#define VG_TEXTURE0                       0x84C0
#define VG_TEXTURE1                       0x84C1
#define VG_TEXTURE2                       0x84C2
#define VG_TEXTURE3                       0x84C3
#define VG_TEXTURE4                       0x84C4
#define VG_TEXTURE5                       0x84C5
#define VG_TEXTURE6                       0x84C6
#define VG_TEXTURE7                       0x84C7
#define VG_TEXTURE8                       0x84C8
#define VG_TEXTURE9                       0x84C9
#define VG_TEXTURE10                      0x84CA
#define VG_TEXTURE11                      0x84CB
#define VG_TEXTURE12                      0x84CC
#define VG_TEXTURE13                      0x84CD
#define VG_TEXTURE14                      0x84CE
#define VG_TEXTURE15                      0x84CF
#define VG_TEXTURE16                      0x84D0
#define VG_TEXTURE17                      0x84D1
#define VG_TEXTURE18                      0x84D2
#define VG_TEXTURE19                      0x84D3
#define VG_TEXTURE20                      0x84D4
#define VG_TEXTURE21                      0x84D5
#define VG_TEXTURE22                      0x84D6
#define VG_TEXTURE23                      0x84D7
#define VG_TEXTURE24                      0x84D8
#define VG_TEXTURE25                      0x84D9
#define VG_TEXTURE26                      0x84DA
#define VG_TEXTURE27                      0x84DB
#define VG_TEXTURE28                      0x84DC
#define VG_TEXTURE29                      0x84DD
#define VG_TEXTURE30                      0x84DE
#define VG_TEXTURE31                      0x84DF
#define VG_ACTIVE_TEXTURE                 0x84E0

/* TextureWrapMode */
#define VG_REPEAT                         0x2901
#define VG_CLAMP_TO_EDGE                  0x812F
#define VG_MIRRORED_REPEAT                0x8370

/* Uniform Types */
#define VG_FLOAT_VEC2                     0x8B50
#define VG_FLOAT_VEC3                     0x8B51
#define VG_FLOAT_VEC4                     0x8B52
#define VG_INT_VEC2                       0x8B53
#define VG_INT_VEC3                       0x8B54
#define VG_INT_VEC4                       0x8B55
#define VG_BOOL                           0x8B56
#define VG_BOOL_VEC2                      0x8B57
#define VG_BOOL_VEC3                      0x8B58
#define VG_BOOL_VEC4                      0x8B59
#define VG_FLOAT_MAT2                     0x8B5A
#define VG_FLOAT_MAT3                     0x8B5B
#define VG_FLOAT_MAT4                     0x8B5C
#define VG_SAMPLER_2D                     0x8B5E
#define VG_SAMPLER_CUBE                   0x8B60

/* Vertex Arrays */
#define VG_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define VG_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define VG_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define VG_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define VG_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define VG_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define VG_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F

/* Read Format */
#define VG_IMPLEMENTATION_COLOR_READ_TYPE   0x8B9A
#define VG_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B

/* Shader Source */
#define VG_COMPILE_STATUS                 0x8B81
#define VG_INFO_LOG_LENGTH                0x8B84
#define VG_SHADER_SOURCE_LENGTH           0x8B88
#define VG_SHADER_COMPILER                0x8DFA

/* Shader Binary */
#define VG_SHADER_BINARY_FORMATS          0x8DF8
#define VG_NUM_SHADER_BINARY_FORMATS      0x8DF9

/* Shader Precision-Specified Types */
#define VG_LOW_FLOAT                  0x8DF0
#define VG_MEDIUM_FLOAT               0x8DF1
#define VG_HIGH_FLOAT                 0x8DF2
#define VG_LOW_INT                    0x8DF3
#define VG_MEDIUM_INT                 0x8DF4
#define VG_HIGH_INT                   0x8DF5

/* Framebuffer Object. */
#define VG_FRAMEBUFFER                                      0x8D40
#define VG_RENDERBUFFER                                     0x8D41

#define VG_RGBA4                                            0x8056
#define VG_RGB5_A1                                          0x8057
#define VG_RGB565                                           0x8D62
#define VG_RGBA8											0x8058
#define VG_DEPTH_COMPONENT16                                0x81A5
#define VG_DEPTH_COMPONENT24                                0x81A6
#define VG_DEPTH_COMPONENT32                                0x81A7
#define VG_STENCIL_INDEX                                    0x1901
#define VG_STENCIL_INDEX8                                   0x8D48

#define VG_RENDERBUFFER_WIDTH                               0x8D42
#define VG_RENDERBUFFER_HEIGHT                              0x8D43
#define VG_RENDERBUFFER_INTERNAL_FORMAT                     0x8D44
#define VG_RENDERBUFFER_RED_SIZE                            0x8D50
#define VG_RENDERBUFFER_GREEN_SIZE                          0x8D51
#define VG_RENDERBUFFER_BLUE_SIZE                           0x8D52
#define VG_RENDERBUFFER_ALPHA_SIZE                          0x8D53
#define VG_RENDERBUFFER_DEPTH_SIZE                          0x8D54
#define VG_RENDERBUFFER_STENCIL_SIZE                        0x8D55

#define VG_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE               0x8CD0
#define VG_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME               0x8CD1
#define VG_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL             0x8CD2
#define VG_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE     0x8CD3

#define VG_COLOR_ATTACHMENT0                                0x8CE0
#define VG_DEPTH_ATTACHMENT                                 0x8D00
#define VG_STENCIL_ATTACHMENT                               0x8D20

#define VG_NONE                                             0

#define VG_FRAMEBUFFER_COMPLETE                             0x8CD5
#define VG_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                0x8CD6
#define VG_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT        0x8CD7
#define VG_FRAMEBUFFER_INCOMPLETE_DIMENSIONS                0x8CD9
#define VG_FRAMEBUFFER_UNSUPPORTED                          0x8CDD

#define VG_FRAMEBUFFER_BINDING                              0x8CA6
#define VG_RENDERBUFFER_BINDING                             0x8CA7
#define VG_MAX_RENDERBUFFER_SIZE                            0x84E8

#define VG_INVALID_FRAMEBUFFER_OPERATION                    0x0506


#include "Graphics/Graphics_impl.h"

#endif // INCLUDE_VXGRAPHICS_H
