/**
 * @file RenderCore.cpp
 * hrenderコア機能部
 */

#ifdef HIVE_WITH_COMPOSITOR
#include <mpi.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RenderCore.h"

#include "Commands.h"
#include "Time.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../RenderObject/RenderObject.h"
#include "../RenderObject/PolygonModel.h"
#include "../RenderObject/PointModel.h"
#include "../RenderObject/LineModel.h"
#include "../RenderObject/VectorModel.h"
#include "../RenderObject/TetraModel.h"
#include "../RenderObject/VolumeModel.h"
#include "../RenderObject/SparseVolumeModel.h"
#include "../RenderObject/Camera.h"
#include "../RenderObject/PolygonModel.h"
#include "../Core/Ref.h"

#include "../Core/vxmath.h"

#include "PolygonBuffer.h"
#include "PointBuffer.h"
#include "LineBuffer.h"
#include "VolumeBuffer.h"
#include "SparseVolumeBuffer.h"
#include "VectorBuffer.h"
#include "TetraBuffer.h"

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferImageData.h"
#include "../Image/ImageSaver.h"

#include "../Core/Path.h"

#ifdef HIVE_WITH_COMPOSITOR
extern "C" {
#include "234compositor.h"
}
#endif


namespace {
    inline std::string make_lowercase(const std::string& in)
    {
        std::string out;
        std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
        return out;
    }
    BufferImageData::FORMAT getFileFomat(const std::string& filename)
    {
        if (filename == "") {
            return BufferImageData::RGBA8;
        }
        
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

/**
 * hrenderコア機能部
 */
class RenderCore::Impl {

private:
    int m_width;
    int m_height;
    RENDER_MODE m_mode;
    
    // Framebuffers
    unsigned int m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer;
    unsigned int m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer;
    VX::Math::vec4 m_clearcolor;
    
    const Camera* m_currentCamera;
    
    // Rendering nodes
    typedef std::vector<RefPtr<RenderObject> > RenderObjectArray;
    RenderObjectArray m_renderObjects;
    
    // Device caches
    /*typedef std::map<const PolygonModel*, RefPtr<PolygonBuffer> > PolygonBufferMap;
    typedef std::map<const PointModel*,   RefPtr<PointBuffer> >   PointBufferMap;
    typedef std::map<const VolumeModel*,  RefPtr<VolumeBuffer> >  VolumeBufferMap;
    PolygonBufferMap m_polygonBuffers;
    PointBufferMap   m_pointBuffers;
    VolumeBufferMap  m_volumeBuffers;*/
    typedef std::map<const std::string, unsigned int> ShaderCache;
    typedef std::map<const BufferImageData*, unsigned int> TextureCache;
    typedef std::map<const RenderObject*, RefPtr<BaseBuffer> > BufferMap;
    BufferMap m_buffers_SGL;
    BufferMap m_buffers_GL;
    TextureCache m_textureCache;
    ShaderCache  m_shaderCache;
    
    ImageSaver m_imagesaver;

#ifdef HIVE_WITH_COMPOSITOR
	int  m_compPixelType;
	bool m_compInitialized;
#endif
    
    double m_renderTimeout;
    double m_oldCallbackTime;
    bool (*m_progressCallback)(double);
    static bool progressCallbackFunc_(int progress, int y, int height, void* ptr) {
        return static_cast<Impl*>(ptr)->progressCallbackFunc(progress, y, height);
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
    static bool defaultProgressCallbackFunc(double progress) {
        printf("[Rendering] %3d%%\n", static_cast<int>(progress));
        return true;
    }
    
    
public:
    /// コンストラクタ
    Impl()
    {
        m_mode   = RENDER_LSGL;//RENDER_OPENGL;
        m_clearcolor = VX::Math::vec4(0,0,0,0); // Always (0,0,0,0). we set clearcolor at readbacked.
        m_sgl_depthbuffer = 0;
        m_sgl_colorbuffer = 0;
        m_sgl_framebuffer = 0;
        m_gl_depthbuffer  = 0;
        m_gl_colorbuffer  = 0;
        m_gl_framebuffer  = 0;

        m_renderTimeout    = 0.2; // sec
        m_oldCallbackTime  = 0.0;
        m_progressCallback = defaultProgressCallbackFunc;
        
#ifndef USE_GLSL_CONFIG
        LSGL_CompilerSetting();
#endif
        SetCallback_SGL(Impl::progressCallbackFunc_, this);
#ifdef HIVE_WITH_COMPOSITOR
		m_compPixelType = ID_RGBA32;
		m_compInitialized = false;
#endif
    }
    
    /// デストラクタ
    ~Impl() {
        ReleaseBuffer_SGL(m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //ReleaseBuffer_GL(m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);
#ifdef HIVE_WITH_COMPOSITOR

#endif
    }
    
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
    
    /// バッファのクリア
    void ClearBuffers()
    {
        m_buffers_SGL.clear();
        m_buffers_GL.clear();
        
        TextureCache::const_iterator it, eit = m_textureCache.end();
        for (it = m_textureCache.begin(); it != eit; ++it) {
            unsigned int t = it->second;
            DeleteTextures_SGL(1, &t);
        }
        m_textureCache.clear();

        ShaderCache::const_iterator sit, seit = m_shaderCache.end();
        for (sit = m_shaderCache.begin(); sit != seit; ++sit) {
            const unsigned int p = sit->second;
            DeleteProgram_SGL(p);
        }
        m_shaderCache.clear();
        
    }
    
    void SetParallelRendering(bool enableParallel)
    {
        SetScreenParallel_SGL(enableParallel, false);
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
        GenTextures_SGL(1, &tex);
        m_textureCache[bufimg] = tex;
        return true;
    }

    bool DeleteTexture(const BufferImageData* bufimg)
    {
        TextureCache::iterator it = m_textureCache.find(bufimg);
        if (it != m_textureCache.end()) {
            DeleteTextures_SGL(1, &it->second);
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
        bool r = CreateProgramSrc_SGL(srcname, prg);
        if (!r)
            return false;
        m_shaderCache[std::string(srcname)] = prg;
        return true;
    }
    
    
    
    /// レンダリング
    void Render()
    {
        m_oldCallbackTime = 0.0;//GetTimeCount();
        RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
        for (it = m_renderObjects.begin(); it != eit; ++it)
        {
            if ((*it)->GetType() == RenderObject::TYPE_CAMERA) {
                Camera* camera = static_cast<Camera*>(it->Get());
                const std::string& outfile = camera->GetOutputFile();
                const std::string& depth_outfile = camera->GetDepthOutputFile();
                BufferImageData* color = camera->GetImageBuffer();
                BufferImageData* depth = camera->GetDepthBuffer();
                
                const double starttm = GetTimeCount();
                resize(camera);
                const double resizetm = GetTimeCount();
                setCurrentCamera(camera);
                renderObjects();
                const double rendertm = GetTimeCount();
                const float* clr = camera->GetClearColor();
                readbackImage(color, clr[0], clr[1], clr[2], clr[3]);
                readbackDepth(depth);
                const double readbacktm = GetTimeCount();

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
                const double savetm = GetTimeCount();
                printf("[HIVE] Resize=%.3f DrawCall=%.3f Readback=%.3f Save=%.3f\n", resizetm-starttm, rendertm-resizetm, readbacktm-rendertm, savetm-readbacktm);
            }
        }
    }
   
private:
    
    /// カレントカメラのセット
    /// @param camera カメラ
    void setCurrentCamera(const Camera* camera)
    {
        m_currentCamera = camera;
    }
    
    /// SGLバッファの作成
    /// @param robj レンダーオブジェクト
    BaseBuffer* createBufferSGL(const RenderObject* robj)
    {
        BaseBuffer* buffer = 0;
        if (robj->GetType() == RenderObject::TYPE_POLYGON) {
            PolygonBuffer* pbuf = new PolygonBuffer(RENDER_LSGL);
            pbuf->Create(static_cast<const PolygonModel*>(robj));
            buffer = pbuf;
        } else if (robj->GetType() == RenderObject::TYPE_POINT) {
            PointBuffer* pbuf = new PointBuffer(RENDER_LSGL);
            pbuf->Create(static_cast<const PointModel*>(robj));
            buffer = pbuf;
        } else if (robj->GetType() == RenderObject::TYPE_LINE) {
            LineBuffer* lbuf = new LineBuffer(RENDER_LSGL);
            lbuf->Create(static_cast<const LineModel*>(robj));
            buffer = lbuf;
        } else if (robj->GetType() == RenderObject::TYPE_VOLUME) {
             VolumeBuffer* vbuf = new VolumeBuffer(RENDER_LSGL);
             vbuf->Create(static_cast<const VolumeModel*>(robj));
             buffer = vbuf;
        } else if (robj->GetType() == RenderObject::TYPE_SPARSEVOLUME) {
             SparseVolumeBuffer* vbuf = new SparseVolumeBuffer(RENDER_LSGL);
             vbuf->Create(static_cast<const SparseVolumeModel*>(robj));
             buffer = vbuf;
        } else if (robj->GetType() == RenderObject::TYPE_TETRA) {
            TetraBuffer* tbuf = new TetraBuffer(RENDER_LSGL);
            tbuf->Create(static_cast<const TetraModel*>(robj));
            buffer = tbuf;
        } else if (robj->GetType() == RenderObject::TYPE_VECTOR) {
             VectorBuffer* vbuf = new VectorBuffer(RENDER_LSGL);
             vbuf->Create(static_cast<const VectorModel*>(robj));
             buffer = vbuf;
        } else {
            // Unknow type
            fprintf(stderr,"Unkown RenderObjet type:\n");
            assert(0);
        }
        return buffer;
    }
    
    /// SGLで描画
    /// @param robj レンダーオブジェクト
    void draw_SGL(const RenderObject* robj)
    {
        if (robj->GetType() == RenderObject::TYPE_CAMERA) {
            return;
        }
        
        BaseBuffer* buffer = 0;
        BufferMap::const_iterator it = m_buffers_SGL.find(robj);
        if (it != m_buffers_SGL.end()) {
            buffer = it->second.Get();
        } else {
            BaseBuffer* buf = createBufferSGL(robj);
            m_buffers_SGL[robj] = buf;
            buffer = buf;
        }

        assert(buffer);

        const float res[] = {m_width, m_height};
        buffer->Update();
        buffer->BindProgram();
        buffer->Uniform2fv("resolution", res);
        buffer->Uniform4fv("backgroundColor", &m_clearcolor.x);
        buffer->SetCamera(m_currentCamera);
        buffer->Render();
        buffer->UnbindProgram();
        
    }
    
    void draw_GL(const RenderObject* robj)
    {
        // TODO
         /// old code
        /* unsigned int prog = (*it)->GetGLProgram();
         if (!prog)
         continue;
         // TODO: move to Render()
         BindProgram_GL(prog);
         SetUniform2fv_GL(prog, "resolution", res);
         SetUniform4fv_GL(prog, "backgroundColor", bgcolor);
         
         SetCamera_GL(prog, m_eye, m_lookat, m_up, m_fov, m_width, m_height, m_near, m_far);
         (*it)->Render(RENDER_OPENGL);*/
    }

    /// 画像の書き戻し
    /// @param color カラーバッファ
    void readbackDepth(BufferImageData* depth)
    {
        FloatBuffer* fbuf = depth->FloatImageBuffer();
        if (fbuf) {
            float* imgbuf = fbuf->GetBuffer();
            GetDepthBuffer_SGL(m_width, m_height, imgbuf);

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
    void readbackImage(BufferImageData* color, float clr_r, float clr_g, float clr_b, float clr_a)
    {
        const float clearcolor_r = clr_r;
        const float clearcolor_g = clr_g;
        const float clearcolor_b = clr_b;
        const float clearcolor_a = clr_a;

        ByteBuffer* bbuf = color->ImageBuffer();
        if (bbuf) {
            unsigned char* imgbuf = bbuf->GetBuffer();
            const int colorbit = 8;
        
            if (m_mode == RENDER_LSGL)
                GetColorBuffer_SGL(m_width, m_height, imgbuf, colorbit);
            //else
            //	GetColorBuffer_GL(m_width, m_height, m_imgbuf);// todo nothing here!
        
#ifdef HIVE_WITH_COMPOSITOR
            int rank;
            int nnodes;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

			if (nnodes > 1) {

				// Assume m_compPixelType == ID_RGBA32
				assert(m_compPixelType == ID_RGBA32);
				Do_234Composition(rank, nnodes, m_width, m_height, m_compPixelType, ALPHA_BtoF, imgbuf, MPI_COMM_WORLD );
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
            float* imgbuf = fbuf->GetBuffer();
            const int colorbit = 32;
            if (m_mode == RENDER_LSGL)
                GetColorBuffer_SGL(m_width, m_height, reinterpret_cast<unsigned char*>(imgbuf), colorbit);

#ifdef HIVE_WITH_COMPOSITOR
            int rank;
            int nnodes;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &nnodes);
            
			if (nnodes > 1) {
				Do_234Composition(rank, nnodes, m_width, m_height, m_compPixelType, ALPHA_BtoF, imgbuf, MPI_COMM_WORLD );
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
    void renderObjects()
    {
        printf("RenderCore::RENDER!!!!\n");
        
        if (m_mode == RENDER_LSGL) {
            Clear_SGL(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w);
            
            //SampleCoverage_SGL(m_fsaa, 0);
            //PixelStep_SGL(m_pixelstep);
            
            RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
            for (it = m_renderObjects.begin(); it != eit; ++it)
            {
                draw_SGL((*it));
            }
            
            //BindProgram_SGL(0); // TODO: not need to release?
        } else {
            //bindGLBuffer();
            Clear_GL(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w);
            
            RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
            for (it = m_renderObjects.begin(); it != eit; ++it)
            {
                draw_GL((*it));
            }
            
            BindProgram_GL(0);
            
            //unbindGLBuffer();
        }
 
    }
    
    /// リサイズ
    /// @param camera カメラ
    void resize(Camera* camera)
    {
        const std::string& outfile = camera->GetOutputFile();
        BufferImageData::FORMAT colorfmt = getFileFomat(outfile);
        
        BufferImageData* color = camera->GetImageBuffer();
        BufferImageData* depth = camera->GetDepthBuffer();
        const int w = camera->GetScreenWidth();
        const int h = camera->GetScreenHeight();
        
        if (m_sgl_framebuffer || m_sgl_colorbuffer || m_sgl_depthbuffer)
            ReleaseBuffer_SGL(m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //		if (m_gl_framebuffer || m_gl_colorbuffer || m_gl_depthbuffer)
        //			ReleaseBuffer_GL(m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer); // todo nothing here
        
        const int colorbit = (colorfmt == BufferImageData::RGBA32F ? 32 : 8);
        CreateBuffer_SGL(w, h, m_sgl_framebuffer, m_sgl_colorbuffer, colorbit, m_sgl_depthbuffer, 32);
        //		CreateBuffer_GL  (w, h, m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);  // todo nothing here

#ifdef HIVE_WITH_COMPOSITOR
        int rank;
        int nnodes;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

        m_compPixelType = (colorfmt == BufferImageData::RGBA32F ? ID_RGBA128 : ID_RGBA32);

        // Re-allocate compositor buffer for the change of screen resolution.
        // @fixme { Support various image format. Currently only RGBA 8bit allowd. }
        if (m_width != w || m_height != h) {
            if (m_width == 0 || m_height ==0) { // Assume first call of resize() function.
            } else {
                Destroy_234Composition(m_compPixelType);
            }
            Init_234Composition (rank, nnodes, w, h, m_compPixelType);
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

    
};

// ----------------------------------------------------

/// インスタンスの取得
RenderCore* RenderCore::GetInstance()
{
	static RenderCore inst;
	return &inst;
}

/// コンストラクタ
RenderCore::RenderCore() : m_imp(new Impl()) {}
/// デストラクタ
RenderCore::~RenderCore()  { delete m_imp; }

/// レンダーオブジェクトの追加
/// @param robj レンダーオブジェクト
void RenderCore::AddRenderObject(RenderObject* robj)
{
    m_imp->AddRenderObject(robj);
}

bool RenderCore::GetTexture(const BufferImageData* bufimg, unsigned int& id)
{
    return m_imp->GetTexture(bufimg, id);
}

bool RenderCore::CreateTexture(const BufferImageData* bufimg, unsigned int& tex)
{
    return m_imp->CreateTexture(bufimg, tex);
}

bool RenderCore::DeleteTexture(const BufferImageData* bufimg)
{
    return m_imp->DeleteTexture(bufimg);
}


/// レンダー
void RenderCore::Render()
{
    m_imp->Render();
}

/// レンダーオブジェクトのクリア
void RenderCore::ClearRenderObject()
{
    m_imp->ClearRenderObject();
}

/// レンダーバッファのクリア
void RenderCore::ClearBuffers()
{
    m_imp->ClearBuffers();
}

void RenderCore::SetParallelRendering(bool enableParallel)
{
    m_imp->SetParallelRendering(enableParallel);
}

/// プログレスコールバックの設定
void RenderCore::SetProgressCallback(bool (*func)(double))
{
    m_imp->SetProgressCallback(func);
}

bool RenderCore::CreateProgramSrc(const char* src, unsigned int& prg)
{
    return m_imp->CreateProgramSrc(src, prg);
}

