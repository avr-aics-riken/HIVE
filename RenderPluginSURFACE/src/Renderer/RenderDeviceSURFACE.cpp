/**
 * @file RenderDevice.cpp
 * hrenderコア機能部
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HIVE_BUILD_WITH_OPENGL
#include "GLDevice/GLDevice.h"
#include "GLDevice/GLDeviceExtention.h"
#endif

#include "Commands.h"
#include "xtime.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "RenderDevice.h"


#include <RenderObject/RenderObject.h>
#include <RenderObject/Camera.h>
#include <Buffer/Buffer.h>
#include <Buffer/BufferImageData.h>
#include <Image/ImageSaver.h>
#include <Core/Path.h>
#include <Core/Perf.h>
#include <Core/vxmath.h>


#ifdef HIVE_WITH_COMPOSITOR
#include <mpi.h> // FIRST, include for MPI-C++ binding
extern "C" {
#include "234compositor.h"
}
#endif


namespace {

    int m_mode = 1; // depcated : TODO: DELETE ME


    ImageSaver m_imagesaver;

    inline std::string make_lowercase(const std::string& in)
    {
        std::string out;
        std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
        return out;
    }
    BufferImageData::FORMAT getImageBufferFomat(const std::string& filename, const std::string &imagebuffer_format)
    {
        if (filename.empty()) {
            // Determinte image buffer format by imagebuffer format.
            if (imagebuffer_format.compare("RGBA32F") == 0) {
                return BufferImageData::RGBA32F;
            } else if (imagebuffer_format.compare("RGBA8") == 0) {
                return BufferImageData::RGBA8;
            }
            
            return BufferImageData::INVALID;
        } else {
            // Determinte image buffer format by filename extension.
    
            std::string::size_type pos = filename.rfind('.');
            if (pos == std::string::npos) {
                return BufferImageData::INVALID;
            }
            
            const std::string ext = make_lowercase(filename.substr(pos + 1));
            if (ext == "jpg" || ext == "png") {
                return BufferImageData::RGBA8;
            } else if (ext == "hdr" || ext == "exr") {
                return BufferImageData::RGBA32F;
            } else {
                return BufferImageData::RGBA8;
            }
        }
    }
    
    bool (* const CreateProgramSrc_GS[])(const char*, unsigned int& prg) = {CreateProgramSrc_GL, CreateProgramSrc_SGL};
    bool (* const DeleteProgram_GS[])(unsigned int prg) = {DeleteProgram_GL, DeleteProgram_SGL};
    void (* const DeleteTextures_GS[])(int n, unsigned int* tex) = {DeleteTextures_GL, DeleteTextures_SGL};
    void (* const GenTextures_GS[])(int n, unsigned int* tex) = {GenTextures_GL, GenTextures_SGL};
    void (* const Clear_GS[])(float r, float g, float b, float a) = {Clear_GL, Clear_SGL};
    void (* const GetColorBuffer_GS[])(int w, int h, unsigned char* imgbuf, int colorbit) = {GetColorBuffer_GL, GetColorBuffer_SGL};
    void (* const GetDepthBuffer_GS[])(int w, int h, float* depthbuf) = {GetDepthBuffer_GL, GetDepthBuffer_SGL};
    
    void (* const CreateBuffer_GS[])(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer, int colorbit, unsigned int& depthRenderbuffer, int depthbit) = {CreateBuffer_GL, CreateBuffer_SGL};
    void (* const ReleaseBuffer_GS[])(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer) = {ReleaseBuffer_GL, ReleaseBuffer_SGL};


    void (* const SetUniform4fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform4fv_GL, SetUniform4fv_SGL};
    void (* const SetUniform3fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform3fv_GL, SetUniform3fv_SGL};
    void (* const SetUniform2fv_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniform2fv_GL, SetUniform2fv_SGL};
    void (* const SetUniform1f_GS[])(unsigned int prg, const char* name, float val) = {SetUniform1f_GL, SetUniform1f_SGL};
    void (* const SetUniform1i_GS[])(unsigned int prg, const char* name, int val) = {SetUniform1i_GL, SetUniform1i_SGL};
    void (* const BindProgram_GS[])(unsigned int prg) = {BindProgram_GL, BindProgram_SGL};
    void (* const SetUniformMatrix_GS[])(unsigned int prg, const char* name, const float* val) = {SetUniformMatrix_GL, SetUniformMatrix_SGL};
    
    void (* const CreateFloatBuffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateFloatBuffer_GL, CreateFloatBuffer_SGL};
    void (* const CreateUintBuffer_GS[])(unsigned int num, unsigned int* buffer, unsigned int& buf_id) = {CreateUintBuffer_GL, CreateUintBuffer_SGL};
    void (* const CreateVec4Buffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateVec4Buffer_GL, CreateVec4Buffer_SGL};
    void (* const CreateVec3Buffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateVec3Buffer_GL, CreateVec3Buffer_SGL};
    void (* const CreateVec2Buffer_GS[])(unsigned int num, float* buffer, unsigned int& buf_id) = {CreateVec2Buffer_GL, CreateVec2Buffer_SGL};
    void (* const BindBufferFloat_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferFloat_GL, BindBufferFloat_SGL};
    void (* const BindBufferUint_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferUint_GL, BindBufferUint_SGL};
    void (* const BindBufferVec4_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferVec4_GL, BindBufferVec4_SGL};
    void (* const BindBufferVec3_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferVec3_GL, BindBufferVec3_SGL};
    void (* const BindBufferVec2_GS[])(unsigned int prg, const char* attrname, unsigned int bufidx) = {BindBufferVec2_GL, BindBufferVec2_SGL};
    void (* const UnBindBuffer_GS[])(unsigned int prg, const char* attrname) = {UnBindBuffer_GL, UnBindBuffer_SGL};

    void (* const BindTexture2D_GS[])(unsigned int texid) = {BindTexture2D_GL, BindTexture2D_SGL};
    void (* const ActiveTexture_GS[])(unsigned int n) = {ActiveTexture_GL, ActiveTexture_SGL};
    
    void (* const TexImage2D_GS[])(unsigned int width, unsigned int height,
                                   unsigned int component, const unsigned char* pixeldata,
                                   bool filter, bool clampToEdgeS, bool clampToEdgeT) = {TexImage2D_GL, TexImage2D_SGL};
 
    void (* const TexImage2DFloat_GS[])(unsigned int width, unsigned int height,
                                        unsigned int component, const float* pixeldata,
                                        bool filter, bool clampToEdgeS, bool clampToEdgeT) = {TexImage2DFloat_GL, TexImage2DFloat_SGL};
 
    
    void (* const ReleaseBufferVBIB_GS[])(unsigned int buffer_id) = {ReleaseBufferVBIB_GL, ReleaseBufferVBIB_SGL};
    void (* const CreateVBIB_GS[])(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                                   float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                                   unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                                   unsigned int& tex_id, unsigned int& index_id) = {CreateVBIB_GL, CreateVBIB_SGL};
    void (* const BindVBIB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
                                 unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) = {BindVBIB_GL, BindVBIB_SGL};
    void (* const UnBindVBIB_GS[])(unsigned int prg) = {UnBindVBIB_GL, UnBindVBIB_SGL};
    void (* const DrawElements_GS[])(unsigned int indexnum) = {DrawElements_GL, DrawElements_SGL};
    void (* const DrawArrays_GS[])(unsigned int vtxnum) = {DrawArrays_GL, DrawArrays_SGL};
    
    void (* const BindLineVBIB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) = {BindLineVBIB_GL, BindLineVBIB_SGL};
    void (* const UnBindLineVBIB_GS[])(unsigned int prg) = {UnBindLineVBIB_GL, UnBindLineVBIB_SGL};
    void (* const DrawLineElements_GS[])(unsigned int indexnum) = {DrawLineElements_GL, DrawLineElements_SGL};
    void (* const DrawLineArrays_GS[])(unsigned int vtxnum) = {DrawLineArrays_GL, DrawLineArrays_SGL};
    void (* const CreateVBRM_GS[])(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                   unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) = {CreateVBRM_GL, CreateVBRM_SGL};
    void (* const CreateVBIBRM_GS[])(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                                     unsigned int indexnum, unsigned int* indexbuffer,
                                     unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id) = {CreateVBIBRM_GL, CreateVBIBRM_SGL};
    void (* const LineWidth_GS[])(float w) = {LineWidth_GL, LineWidth_SGL};
    void (* const BindPointVB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) = {BindPointVB_GL, BindPointVB_SGL};
    void (* const UnBindPointVB_GS[])(unsigned int prg) = {UnBindPointVB_GL, UnBindPointVB_SGL};
    void (* const DrawPointArrays_GS[])(unsigned int vtxnum) = {DrawPointArrays_GL, DrawPointArrays_SGL};
    
    
    void (* const BindTetraVBIB_GS[])(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) = {BindTetraVBIB_GL, BindTetraVBIB_SGL};
    void (* const UnBindTetraVBIB_GS[])(unsigned int prg) = {UnBindTetraVBIB_GL, UnBindTetraVBIB_SGL};
    void (* const DrawTetraArrays_GS[])(unsigned int vtxnum) = {DrawTetraArrays_GL, DrawTetraArrays_SGL};


    /// LSGLコンパイラセッティング
    void LSGL_CompilerSetting()
    {
        std::string binaryPath = getBinaryDir();
    #ifdef __APPLE__
        std::string binpath = "macosx64";
        std::string ccmd    = "clang++";
    #elif _WIN32
        std::string binpath = "win64";
        std::string ccmd    = "g++";
    #elif defined(__sparc__) || defined(__sparc_v9__)
        std::string binpath = "sparc64";
    #ifdef HIVE_ENABLE_MPI
        std::string ccmd    = "mpiFCC";
    #else
        std::string ccmd    = "FCC";
    #endif
    #elif __linux__
        std::string binpath = "linux_x64";
        std::string ccmd    = "g++";
    #endif
        std::string opt      = "-O2";
        
        std::string mesaPath = binaryPath + "glsl/bin/" + binpath + "/glsl_compiler";
        std::string compilerCmd;
    #ifdef _WIN32
        compilerCmd += binaryPath + std::string("glsl\\glslc.bat");
    #else
        compilerCmd += binaryPath + std::string("glsl/glslc");
    #endif
        compilerCmd += std::string(" --cxx=\"")      + ccmd     + std::string("\"");
        compilerCmd += std::string(" --cxxflags=\"") + opt      + std::string("\"");
        compilerCmd += std::string(" --mesacc=\"")   + mesaPath + std::string("\"");
        SetShaderCompiler_SGL(compilerCmd.c_str(), NULL);
    }

}

class RenderDevice::Impl {

private:
    // Rendering nodes
    typedef std::vector<RefPtr<RenderObject> > RenderObjectArray;
    RenderObjectArray m_renderObjects;
    
#ifdef HIVE_WITH_COMPOSITOR
	int  m_compPixelType;
	bool m_compInitialized;
#endif

    double m_renderTimeout;
    double m_oldCallbackTime;
    bool (*m_progressCallback)(double);
        
    int m_width;
    int m_height;
    float m_clearcolor[4];
    const Camera* m_currentCamera;

    // Use depth layer for 234composition.
    bool m_use_depth_234composition;
	// merge id for 234composition.
	int m_compositionMergeID;

    // Framebuffers
    unsigned int m_gs_framebuffer, m_gs_colorbuffer, m_gs_depthbuffer;
 
     // Device caches
    typedef std::map<const std::string, unsigned int> ShaderCache;
    typedef std::map<const BufferImageData*, unsigned int> TextureCache;
    typedef std::map<const RenderObject*, RefPtr<BaseBuffer> > BufferMap;
    BufferMap m_buffers;
    TextureCache m_textureCache;
    ShaderCache  m_shaderCache;
/*
    /// @param robj レンダーオブジェクト
    void draw(const RenderObject* robj);

    /// カレントカメラのセット
    /// @param camera カメラ
    void setCurrentCamera(const Camera* camera);

    /// 画像の書き戻し
    /// @param color カラーバッファ
    void readbackDepth(BufferImageData* depth);

    /// 画像の書き戻し
    /// @param color カラーバッファ
    void readbackImage(BufferImageData::FORMAT format, BufferImageData* color, float clr_r, float clr_g, float clr_b, float clr_a);

    /// オブジェクトのレンダリング
    void renderObjects();
    
    /// リサイズ
    /// @param camera カメラ
    void resize(Camera* camera);
*/

    
    static bool progressCallbackFunc_(int progress, int y, int height, void* ptr) {
        Impl* rp = reinterpret_cast<Impl*>(ptr);
        return rp->progressCallbackFunc(progress, y, height);
    }
    
    static bool defaultProgressCallbackFunc(double progress) {
        printf("[Rendering] %3d%%\n", static_cast<int>(progress));
        return true;
    }

public:

    
    /// コンストラクタ
    Impl()
    {
        m_mode       = 1;
        m_use_depth_234composition = false;
        m_compositionMergeID = 0; // ALPHA

        m_clearcolor[0] = 0; // Always (0,0,0,0). we set clearcolor at readbacked.
        m_clearcolor[1] = 0;
        m_clearcolor[2] = 0;
        m_clearcolor[3] = 0;
        m_gs_depthbuffer = 0;
        m_gs_colorbuffer = 0;
        m_gs_framebuffer = 0;

        m_renderTimeout    = 0.2; // sec
        m_oldCallbackTime  = 0.0;
        m_progressCallback = Impl::defaultProgressCallbackFunc;
        
    #ifndef USE_GLSL_CONFIG
        LSGL_CompilerSetting();
    #endif
        SetCallback_SGL(Impl::progressCallbackFunc_, this);
        
        
    /*    if (m_mode == RENDER_OPENGL) {
    #ifdef HIVE_BUILD_WITH_OPENGL
            printf("Start OpenGL mode\n");
            GLDevice* dev = CreateGLDeviceInstance();
            bool r = dev->Init(256, 256, 32, 16, true);
            if (!r) {
                printf("[Error] Failed to initialize OpenGL mode\n");
                m_mode = RENDER_SURFACE;
            }
    #else
            printf("[Error] Not Support OpenGL mode\n");
            m_mode = RENDER_SURFACE;
    #endif
        }*/
        
    #ifdef HIVE_WITH_COMPOSITOR
        m_compPixelType = ID_RGBA32;
        m_compInitialized = false;
    #endif
    }

    /// デストラクタ
    ~Impl() {
        ReleaseBuffer_GS[m_mode](m_gs_framebuffer, m_gs_colorbuffer, m_gs_depthbuffer);
        
    #ifdef HIVE_WITH_COMPOSITOR

    #endif
    }

    bool progressCallbackFunc(int progress, int y, int height) {
        const double tm = GetTimeCount();
        const int minimumRenderingHeight = 16; // TODO: Now, FORCE rendering minimum size for Interactive rendring.
        if (height > minimumRenderingHeight
        && (tm - m_oldCallbackTime > m_renderTimeout)) {
            m_oldCallbackTime = tm;
            if (!m_progressCallback)
                return true;
            return m_progressCallback(static_cast<double>(progress));
        }
        return true;
    }

    int GetWidth() const
    {
        return m_width;
    }

    int GetHeight() const
    {
        return m_height;
    }

    /// バッファのクリア
    void ClearBuffers()
    {
        m_buffers.clear();
        
        TextureCache::const_iterator it, eit = m_textureCache.end();
        for (it = m_textureCache.begin(); it != eit; ++it) {
            unsigned int t = it->second;
            DeleteTextures_GS[m_mode](1, &t);
        }
        m_textureCache.clear();

        ShaderCache::const_iterator sit, seit = m_shaderCache.end();
        for (sit = m_shaderCache.begin(); sit != seit; ++sit) {
            const unsigned int p = sit->second;
            DeleteProgram_GS[m_mode](p);
        }
        m_shaderCache.clear();
        
    }

    /// レンダーオブジェクトの追加
    /// @param robj レンダーオブジェクト
    void AddRenderObject(RenderObject* robj)
    {
        m_renderObjects.push_back(robj);
    }

    /// レンダーオブジェクトのクリア
    void ClearRenderObject()
    {
        m_renderObjects.clear();
    }

    /// プログレスコールバックの設定
    void SetProgressCallback(bool (*func)(double))
    {
        m_progressCallback = func;
    }

    bool GetTexture(const BufferImageData* bufimg, unsigned int& id)
    {
        TextureCache::const_iterator it = m_textureCache.find(bufimg);
        if (it != m_textureCache.end()) {
            id = it->second;
            return true;
        }
        return false;
    }

    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex)
    {
        TextureCache::const_iterator it = m_textureCache.find(bufimg);
        if (it != m_textureCache.end()) {
            DeleteTexture(bufimg);
        }
        GenTextures_GS[m_mode](1, &tex);
        m_textureCache[bufimg] = tex;
        return true;
    }

    bool DeleteTexture(const BufferImageData* bufimg)
    {
        TextureCache::iterator it = m_textureCache.find(bufimg);
        if (it != m_textureCache.end()) {
            DeleteTextures_GS[m_mode](1, &it->second);
            m_textureCache.erase(it);
            return true;
        }
        return false;
    }

    bool CreateProgramSrc(const char* srcname, unsigned int& prg)
    {
        ShaderCache::const_iterator it = m_shaderCache.find(srcname);
        if (it != m_shaderCache.end()) {
            prg = it->second;
            return true;
        }
        bool r = CreateProgramSrc_GS[m_mode](srcname, prg);
        if (!r)
            return false;
        m_shaderCache[std::string(srcname)] = prg;
        return true;
    }

    bool ClearShaderCache(const char* srcname)
    {
        ShaderCache::iterator it = m_shaderCache.find(srcname);
        if (it != m_shaderCache.end()) {
            m_shaderCache.erase(it);
        }
        return true;
    }

    /// レンダリング
    void Render(RenderDevice* render)
    {
        m_oldCallbackTime = 0.0;//GetTimeCount();
        RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
        for (it = m_renderObjects.begin(); it != eit; ++it)
        {
            if ((*it)->GetType() == RenderObject::TYPE_CAMERA) {
                PMon::Start("RenderDevice::Render");
                Camera* camera = static_cast<Camera*>(it->Get());
                const std::string& outfile = camera->GetOutputFile();
                const std::string& depth_outfile = camera->GetDepthOutputFile();
                const std::string& imagebuffer_format = camera->GetImageBufferFormat();
                BufferImageData::FORMAT colorfmt = getImageBufferFomat(outfile, imagebuffer_format);
                BufferImageData* color = camera->GetImageBuffer();
                BufferImageData* depth = camera->GetDepthBuffer();
                
                const double starttm = GetTimeCount();
                resize(camera);
                const double resizetm = GetTimeCount();
                setCurrentCamera(camera);
                renderObjects(render);
                PMon::Stop("RenderDevice::Render");
                const double rendertm = GetTimeCount();
                const float* clr = camera->GetClearColor();
    #if defined(HIVE_WITH_COMPOSITOR)
                PMon::Start("Compositor");
    #endif
                if (m_use_depth_234composition) {
					if (depth->Format() ==  BufferImageData::R32F && depth->FloatImageBuffer()) {
						readbackDepth(depth);
						const float *depthData = depth->FloatImageBuffer()->GetBuffer();
						assert(depthDeta);
						readbackImage(colorfmt, color, depthData, clr[0], clr[1], clr[2], clr[3]);
					} else {
						printf("[Error] Failed to composite depth buffer\n");
					}
                } else {
                    readbackImage(colorfmt, color, NULL, clr[0], clr[1], clr[2], clr[3]);
                    readbackDepth(depth);
                }
    #if defined(HIVE_WITH_COMPOSITOR)
                PMon::Stop("Compositor");
    #endif
                const double readbacktm = GetTimeCount();

                PMon::Start("RenderDevice::ImageSave");

    #ifdef HIVE_ENABLE_MPI
                int rank = 0;
                MPI_Comm_rank(MPI_COMM_WORLD, &rank);
                if (rank == 0) {
    #endif
                
                if (!outfile.empty()) {
                    m_imagesaver.Save(outfile.c_str(), color);
                }
                if (!depth_outfile.empty()) {
                    m_imagesaver.Save(depth_outfile.c_str(), depth);
                }
                    
    #ifdef HIVE_ENABLE_MPI
                }
    #endif
                PMon::Stop("RenderDevice::ImageSave");
                const double savetm = GetTimeCount();
                //printf("[HIVE] Resize=%.3f DrawCall=%.3f Readback=%.3f Save=%.3f\n", resizetm-starttm, rendertm-resizetm, readbacktm-rendertm, savetm-readbacktm);
            }
        }
    }

	void EnableDepthComposition(bool enable)
	{
		m_use_depth_234composition = enable;
	}

	void SetCompositionMergeID(int mergeID)
	{
		m_compositionMergeID = mergeID;
	}

private:

    /// カレントカメラのセット
    /// @param camera カメラ
    void setCurrentCamera(const Camera* camera)
    {
        m_currentCamera = camera;
    }


    /// 画像の書き戻し
    /// @param color カラーバッファ
    void readbackDepth(BufferImageData* depth)
    {
        FloatBuffer* fbuf = depth->FloatImageBuffer();
        if (fbuf) {
            float* imgbuf = fbuf->GetBuffer();
            GetDepthBuffer_GS[m_mode](m_width, m_height, imgbuf);

    #ifdef HIVE_WITH_COMPOSITOR
            // 234 compositor does not support Z only compositing at this time.
            // Thus we simply use MPI_Reduce o merge image.
            
            int rank;
            int nnodes;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

            if (nnodes > 1) {

                int n = m_width * m_height;
                std::vector<float> buf(n);
                memcpy(&buf.at(0), imgbuf, n * sizeof(float));

                MPI_Barrier(MPI_COMM_WORLD);

                // Assume screen parallel rendering(i.e. no image overlapping),
                int ret = MPI_Reduce(&buf.at(0), reinterpret_cast<void*>(imgbuf), n, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);

                MPI_Barrier(MPI_COMM_WORLD);
            }

    #endif

        }
    }
    /// 画像の書き戻し
    /// @param color カラーバッファ
    void readbackImage(BufferImageData::FORMAT format, BufferImageData* color, const float *depth, float clr_r, float clr_g, float clr_b, float clr_a)
    {
        const float clearcolor_r = clr_r;
        const float clearcolor_g = clr_g;
        const float clearcolor_b = clr_b;
        const float clearcolor_a = clr_a;

        if (format == BufferImageData::RGBA8) {
            ByteBuffer* bbuf = color->ImageBuffer();
            if (!bbuf) { return; }
            unsigned char* imgbuf = bbuf->GetBuffer();
            const int colorbit = 8;
            GetColorBuffer_GS[m_mode](m_width, m_height, imgbuf, colorbit);
        
    #ifdef HIVE_WITH_COMPOSITOR
            int rank;
            int nnodes;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

            if (nnodes > 1) {

                // Assume m_compPixelType == ID_RGBA32
                assert(m_compPixelType == ID_RGBA32);
                if (depth) {
                    Do_234ZComposition(rank, nnodes, m_width, m_height,
                                      ID_RGBAZ64, m_compositionMergeID, imgbuf, depth,
                                      MPI_COMM_WORLD);
                } else {
                    Do_234Composition(rank, nnodes, m_width, m_height,
                                      m_compPixelType, m_compositionMergeID, imgbuf,
                                      MPI_COMM_WORLD);
                }
            }
    #endif

            // merge to bgcolor
            for (int y = 0; y < m_height; ++y) {
                for (int x = 0; x < m_width; ++x) {
                    const double alp = imgbuf[4*(x + y * m_width) + 3]/255.0;
                    imgbuf[4*(x + y * m_width) + 0] = imgbuf[4*(x + y * m_width) + 0] * alp + 255.0*clearcolor_r*clearcolor_a * (1.0 - alp);
                    imgbuf[4*(x + y * m_width) + 1] = imgbuf[4*(x + y * m_width) + 1] * alp + 255.0*clearcolor_g*clearcolor_a * (1.0 - alp);
                    imgbuf[4*(x + y * m_width) + 2] = imgbuf[4*(x + y * m_width) + 2] * alp + 255.0*clearcolor_b*clearcolor_a * (1.0 - alp);
                    imgbuf[4*(x + y * m_width) + 3] = (std::max)(0, (std::min)(255, static_cast<int>(255 * (alp + clearcolor_a))));
                }
            }
        } else {
            FloatBuffer* fbuf = color->FloatImageBuffer();
            if (!fbuf) { return; }
            float* imgbuf = fbuf->GetBuffer();
            const int colorbit = 32;
            GetColorBuffer_GS[m_mode](m_width, m_height, reinterpret_cast<unsigned char*>(imgbuf), colorbit);

    #ifdef HIVE_WITH_COMPOSITOR
            int rank;
            int nnodes;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &nnodes);
            
            if (nnodes > 1) {
				// Assume m_compPixelType == ID_RGBA128
				assert(m_compPixelType == ID_RGBA128);
                if (depth) {
                    Do_234ZComposition(rank, nnodes, m_width, m_height,
                                      ID_RGBAZ160, m_compositionMergeID, imgbuf, depth,
                                      MPI_COMM_WORLD);
                } else {
                    Do_234Composition(rank, nnodes, m_width, m_height,
                                      m_compPixelType, m_compositionMergeID, imgbuf,
                                      MPI_COMM_WORLD);
                }
            }
    #endif
            
            // merge to bgcolor
            for (int y = 0; y < m_height; ++y) {
                for (int x = 0; x < m_width; ++x) {
                    const double alp = imgbuf[4*(x + y * m_width) + 3];
                    const float R = imgbuf[4*(x + y * m_width) + 0] * alp + clearcolor_r * clearcolor_a * (1.0 - alp);
                    const float G = imgbuf[4*(x + y * m_width) + 1] * alp + clearcolor_g * clearcolor_a * (1.0 - alp);
                    const float B = imgbuf[4*(x + y * m_width) + 2] * alp + clearcolor_b * clearcolor_a * (1.0 - alp);
                    imgbuf[4*(x + y * m_width) + 0] = R;
                    imgbuf[4*(x + y * m_width) + 1] = G;
                    imgbuf[4*(x + y * m_width) + 2] = B;
                    imgbuf[4*(x + y * m_width) + 3] = alp + clearcolor_a;
                }
            }
        }

    }

    /// オブジェクトのレンダリング
    void renderObjects(RenderDevice* render)
    {
        Clear_GS[m_mode](m_clearcolor[0], m_clearcolor[1], m_clearcolor[2], m_clearcolor[3]);
        
        RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
        for (it = m_renderObjects.begin(); it != eit; ++it)
        {
            draw(render, (*it));
        }
        
        //BindProgram_SGL(0); // TODO: not need to release?    
    }

    /// リサイズ
    /// @param camera カメラ
    void resize(Camera* camera)
    {
        const std::string& outfile = camera->GetOutputFile();
        const std::string& imagebuffer_format = camera->GetImageBufferFormat();
        BufferImageData::FORMAT colorfmt = getImageBufferFomat(outfile, imagebuffer_format);
        
        BufferImageData* color = camera->GetImageBuffer();
        BufferImageData* depth = camera->GetDepthBuffer();
        const int w = camera->GetScreenWidth();
        const int h = camera->GetScreenHeight();
        
        ByteBuffer* bbuf = color->ImageBuffer();
        if (w == m_width && h == m_height && bbuf != 0) {
            return;
        }

        if (m_gs_framebuffer || m_gs_colorbuffer || m_gs_depthbuffer)
            ReleaseBuffer_GS[m_mode](m_gs_framebuffer, m_gs_colorbuffer, m_gs_depthbuffer);
        
        const int colorbit = (colorfmt == BufferImageData::RGBA32F ? 32 : 8);
        CreateBuffer_GS[m_mode](w, h, m_gs_framebuffer, m_gs_colorbuffer, colorbit, m_gs_depthbuffer, 32);

    #ifdef HIVE_WITH_COMPOSITOR
        int rank;
        int nnodes;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

        m_compPixelType = (colorfmt == BufferImageData::RGBA32F ? ID_RGBA128 : ID_RGBA32);

        // Re-allocate compositor buffer for the change of screen resolution.
        // @fixme { Support various image format. Currently only RGBA 8bit allowd. }
		if (m_width != w || m_height != h)
		{
			if (m_width == 0 || m_height == 0)
			{ // Assume first call of resize() function.
			}
			else
			{
				Destroy_234Composition(m_compPixelType);
			}
			if (m_use_depth_234composition)
			{
				Init_234Composition(rank, nnodes, w, h,
									((m_compPixelType == ID_RGBA32)
										 ? ID_RGBAZ64
										 : ID_RGBAZ160));
			}
			else
			{
				Init_234Composition(rank, nnodes, w, h, m_compPixelType);
			}
			m_compInitialized = true;
		}
    #endif
        
        m_width  = w;
        m_height = h;
        
        if (color->Width() != w || color->Height() != h) {
            color->Clear();
            depth->Clear();
            if (w != 0 && h != 0) {
                if (colorfmt == BufferImageData::INVALID) {
                    colorfmt = BufferImageData::RGBA8;
                }
                color->Create(colorfmt, w, h);
                depth->Create(BufferImageData::R32F,  w, h);
            }
        }
    }

    /// SGLで描画
    /// @param robj レンダーオブジェクト
    void draw(RenderDevice* render, const RenderObject* robj)
    {
        if (robj->GetType() == RenderObject::TYPE_CAMERA) {
            return;
        }
        
        BaseBuffer* buffer = 0;
        BufferMap::const_iterator it = m_buffers.find(robj);
        if (it != m_buffers.end()) {
            buffer = it->second.Get();
        } else {
            BaseBuffer* buf = render->createBuffer(robj);
            m_buffers[robj] = buf;
            buffer = buf;
        }

        assert(buffer);

        const float res[] = {static_cast<float>(GetWidth()), static_cast<float>(GetHeight())};
        buffer->Update();
        buffer->BindProgram();
        buffer->Uniform2fv("resolution", res);
        buffer->Uniform4fv("backgroundColor", &m_clearcolor[0]);
        buffer->SetCamera(m_currentCamera);
        buffer->Render();
        buffer->UnbindProgram();
        
    }

};


/**
 * RenderDeviceSURFACE
 */
RenderDevice::RenderDevice() : m_imp(new Impl()) {}

RenderDevice::~RenderDevice() {
    delete m_imp;
}

void RenderDevice::ClearBuffers() {
    m_imp->ClearBuffers();
}
void RenderDevice::AddRenderObject(RenderObject* robj){
    m_imp->AddRenderObject(robj);
}
void RenderDevice::ClearRenderObject()
{
    m_imp->ClearRenderObject();
}
void RenderDevice::SetProgressCallback(bool (*func)(double))
{
    m_imp->SetProgressCallback(func);
}
void RenderDevice::EnableDepthComposition(bool enable)
{
	m_imp->EnableDepthComposition(enable);
}

void RenderDevice::SetCompositionMergeID(int mergeID)
{
	m_imp->SetCompositionMergeID(mergeID);
}

bool RenderDevice::GetTexture(const BufferImageData* bufimg, unsigned int& id)
{
    return m_imp->GetTexture(bufimg, id);
}
bool RenderDevice::CreateTexture(const BufferImageData* bufimg, unsigned int& tex)
{
    return m_imp->CreateTexture(bufimg, tex);
}
bool RenderDevice::DeleteTexture(const BufferImageData* bufimg)
{
    return m_imp->DeleteTexture(bufimg);
}
bool RenderDevice::CreateProgramSrc(const char* srcname, unsigned int& prg)
{
    return m_imp->CreateProgramSrc(srcname, prg);
}
bool RenderDevice::ClearShaderCache(const char* srcname) {
    return m_imp->ClearShaderCache(srcname);
};
void RenderDevice::Render()         {
    m_imp->Render(this);
}
int RenderDevice::GetWidth()  const {
    return m_imp->GetWidth();
}
int RenderDevice::GetHeight() const {
    return m_imp->GetHeight();
}
bool RenderDevice::progressCallbackFunc(int progress, int y, int height) const {
    return m_imp->progressCallbackFunc(progress, y, height);
}

//----------------

void RenderDevice::ReleaseBuffer(unsigned int bufferId) const
{
    ReleaseBufferVBIB_GS[m_mode](bufferId);
}
void RenderDevice::CreateVBIB(unsigned int vertexnum, float* posbuffer, float* normalbuffer, float* matbuffer,
                float* texbuffer, unsigned int indexnum, unsigned int* indexbuffer,
                unsigned int& vtx_id, unsigned int& normal_id, unsigned int& mat_id,
                unsigned int& tex_id, unsigned int& index_id) const
{
    CreateVBIB_GS[m_mode](vertexnum, posbuffer, normalbuffer, matbuffer,
                          texbuffer, indexnum, indexbuffer,
                          vtx_id, normal_id, mat_id,
                          tex_id, index_id);
}
void RenderDevice::BindVBIB(unsigned int prg, unsigned int vtxidx, unsigned int normalidx,
              unsigned int vtx_material, unsigned int texidx, unsigned int indexidx) const
{
    BindVBIB_GS[m_mode](prg, vtxidx, normalidx, vtx_material, texidx, indexidx);
}
void RenderDevice::UnBindVBIB(unsigned int prg) const
{
    UnBindVBIB_GS[m_mode](prg);
}
void RenderDevice::DrawElements(unsigned int indexnum) const
{
    DrawElements_GS[m_mode](indexnum);
}
void RenderDevice::DrawArrays(unsigned int vtxnum) const
{
    DrawArrays_GS[m_mode](vtxnum);
}
void RenderDevice::GenTextures(int n, unsigned int* tex) const
{
    GenTextures_GS[m_mode](n, tex);
}
void RenderDevice::BindTexture3D(unsigned int tex) const
{
    BindTexture3D_SGL(tex);
}
void RenderDevice::TexImage3DPointer(unsigned int width, unsigned int height, unsigned int depth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const
{
    TexImage3DPointer_SGL(width, height, depth, component, volumedata, clampToEdgeS, clampToEdgeT, clampToEdgeR);
}

void RenderDevice::SetUniform4fv(unsigned int prg, const char* name, const float* val) const
{
    SetUniform4fv_GS[m_mode](prg, name, val);
}
void RenderDevice::SetUniform3fv(unsigned int prg, const char* name, const float* val) const
{
    SetUniform3fv_GS[m_mode](prg, name, val);
}
void RenderDevice::SetUniform2fv(unsigned int prg, const char* name, const float* val) const
{
    SetUniform2fv_GS[m_mode](prg, name, val);
}
void RenderDevice::SetUniform1f(unsigned int prg, const char* name, float val) const
{
    SetUniform1f_GS[m_mode](prg, name, val);
}
void RenderDevice::BindProgram(unsigned int prg) const
{
    BindProgram_GS[m_mode](prg);
}
void RenderDevice::SetUniformMatrix(unsigned int prg, const char* name, const float* val) const
{
    SetUniformMatrix_GS[m_mode](prg, name, val);
}

void RenderDevice::BindTexture2D(unsigned int texid) const
{
    BindTexture2D_GS[m_mode](texid);
}

void RenderDevice::ActiveTexture(unsigned int n) const
{
    
}

void RenderDevice::TexImage2D(unsigned int width, unsigned int height,
                unsigned int component, const unsigned char* pixeldata,
                bool filter, bool clampToEdgeS, bool clampToEdgeT) const
{
    TexImage2D_GS[m_mode](width, height, component, pixeldata,
                          filter, clampToEdgeS, clampToEdgeT);
}

void RenderDevice::TexImage2DFloat(unsigned int width, unsigned int height,
                     unsigned int component, const float* pixeldata,
                     bool filter, bool clampToEdgeS, bool clampToEdgeT) const
{
    TexImage2DFloat_GS[m_mode](width, height, component, pixeldata,
                           filter, clampToEdgeS, clampToEdgeT);
}

void RenderDevice::SetUniform1i(unsigned int prg, const char* name, int val) const
{
    SetUniform1i_GS[m_mode](prg, name, val);
}
void RenderDevice::TexCoordRemap3D(int axis, int n, const float* values) const
{
    TexCoordRemap3D_SGL(axis, n, values);
}
void RenderDevice::BindLineVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material, unsigned int indexidx) const
{
    BindLineVBIB_GS[m_mode](prg, vtxidx, vtx_radius, vtx_material, indexidx);
}
void RenderDevice::UnBindLineVBIB(unsigned int prg) const
{
    UnBindLineVBIB_GS[m_mode](prg);
}
void RenderDevice::DrawLineElements(unsigned int indexnum) const
{
    DrawLineElements_GS[m_mode](indexnum);
}
void RenderDevice::DrawLineArrays(unsigned int vtxnum) const
{
    DrawLineArrays_GS[m_mode](vtxnum);
}
void RenderDevice::CreateVBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id) const
{
    CreateVBRM_GS[m_mode](vertexnum, posbuffer, radiusbuffer, matbuffer, vtx_id, radius_id, mat_id);
}
void RenderDevice::CreateVBIBRM(unsigned int vertexnum, float* posbuffer, float* radiusbuffer, float* matbuffer,
                  unsigned int indexnum, unsigned int* indexbuffer,
                  unsigned int& vtx_id, unsigned int& radius_id, unsigned int& mat_id, unsigned int& index_id) const
{
    CreateVBIBRM_GS[m_mode](vertexnum, posbuffer, radiusbuffer, matbuffer,
                            indexnum, indexbuffer,
                            vtx_id, radius_id, mat_id, index_id);
}
void RenderDevice::LineWidth(float w) const
{
    LineWidth_GS[m_mode](w);
}
void RenderDevice::BindPointVB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_radius, unsigned int vtx_material) const
{
    BindPointVB_GS[m_mode](prg, vtxidx, vtx_radius, vtx_material);
}
void RenderDevice::UnBindPointVB(unsigned int prg) const
{
    UnBindPointVB_GS[m_mode](prg);
}
void RenderDevice::DrawPointArrays(unsigned int vtxnum) const
{
    DrawPointArrays_GS[m_mode](vtxnum);
}
void RenderDevice::BindTetraVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const
{
    BindTetraVBIB_GS[m_mode](prg, vtxidx, vtx_material, indexidx);
}
void RenderDevice::UnBindTetraVBIB(unsigned int prg) const
{
    UnBindTetraVBIB_GS[m_mode](prg);
}
void RenderDevice::DrawTetraArrays(unsigned int vtxnum) const
{
    DrawTetraArrays_GS[m_mode](vtxnum);
}
void RenderDevice::BindSolidVBIB(unsigned int prg, unsigned int vtxidx, unsigned int vtx_material, unsigned int indexidx) const
{
    BindSolidVBIB_SGL(prg, vtxidx, vtx_material, indexidx);
}
void RenderDevice::UnBindSolidVBIB(unsigned int prg) const
{
    UnBindSolidVBIB_SGL(prg);
}
void RenderDevice::DrawSolidArrays(int solidType, unsigned int vtxnum) const
{
    DrawSolidArrays_SGL(solidType, vtxnum);
}
void RenderDevice::SparseTexImage3DPointer(int level, unsigned int xoffset, unsigned int yoffset, unsigned int zoffset, unsigned int width, unsigned int height, unsigned int depth, unsigned int cellWidth, unsigned int cellHeight, unsigned int cellDepth, unsigned int component, const float* volumedata, bool clampToEdgeS, bool clampToEdgeT, bool clampToEdgeR) const
{
    SparseTexImage3DPointer_SGL(level, xoffset, yoffset, zoffset,
                                width, height, depth, cellWidth, cellHeight, cellDepth,
                                component, volumedata, clampToEdgeS, clampToEdgeT, clampToEdgeT);
}

void RenderDevice::SetCamera(unsigned int prg, const float* eye, const float* lookat, const float* up, float fov) const
{
    SetCamera_SGL(prg, eye, lookat, up, fov);
}
void RenderDevice::CreateFloatBuffer(unsigned int num, float* buffer, unsigned int& buf_id) const
{
    CreateFloatBuffer_GS[m_mode](num, buffer, buf_id);
}
void RenderDevice::CreateUintBuffer(unsigned int num, unsigned int* buffer, unsigned int& buf_id) const
{
    CreateUintBuffer_GS[m_mode](num, buffer, buf_id);
}
void RenderDevice::CreateVec4Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const
{
    CreateVec4Buffer_GS[m_mode](num, buffer, buf_id);
}
void RenderDevice::CreateVec3Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const
{
    CreateVec3Buffer_GS[m_mode](num, buffer, buf_id);
}
void RenderDevice::CreateVec2Buffer(unsigned int num, float* buffer, unsigned int& buf_id) const
{
    CreateVec2Buffer_GS[m_mode](num, buffer, buf_id);
}

void RenderDevice::BindBufferFloat(unsigned int prg, const char* attrname, unsigned int bufidx) const
{
    BindBufferFloat_GS[m_mode](prg, attrname, bufidx);
}
void RenderDevice::BindBufferUint(unsigned int prg, const char* attrname, unsigned int bufidx) const
{
    BindBufferUint_GS[m_mode](prg, attrname, bufidx);
}
void RenderDevice::BindBufferVec4(unsigned int prg, const char* attrname, unsigned int bufidx) const
{
    BindBufferVec4_GS[m_mode](prg, attrname, bufidx);
}
void RenderDevice::BindBufferVec3(unsigned int prg, const char* attrname, unsigned int bufidx) const
{
    BindBufferVec3_GS[m_mode](prg, attrname, bufidx);
}
void RenderDevice::BindBufferVec2(unsigned int prg, const char* attrname, unsigned int bufidx) const
{
    BindBufferVec2_GS[m_mode](prg, attrname, bufidx);
}
void RenderDevice::UnBindBuffer(unsigned int prg, const char* attrname) const
{
    UnBindBuffer_GS[m_mode](prg, attrname);
}


