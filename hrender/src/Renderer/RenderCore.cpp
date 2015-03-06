#ifdef HIVE_WITH_COMPOSITOR
#include <mpi.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RenderCore.h"

#include "Commands.h"
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
#include "../RenderObject/Camera.h"
#include "../RenderObject/PolygonModel.h"
#include "../Core/Ref.h"

#include "../Core/vxmath.h"

#include "PolygonBuffer.h"
#include "PointBuffer.h"
#include "LineBuffer.h"
#include "VolumeBuffer.h"
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
    typedef std::map<const RenderObject*, RefPtr<BaseBuffer> > BufferMap;
    BufferMap m_buffers_SGL;
    BufferMap m_buffers_GL;
    
    ImageSaver m_imagesaver;
    
public:
    Impl()
    {
        m_mode   = RENDER_LSGL;//RENDER_OPENGL;
        m_clearcolor = VX::Math::vec4(0,0,0,0);
        m_sgl_depthbuffer = 0;
        m_sgl_colorbuffer = 0;
        m_sgl_framebuffer = 0;
        m_gl_depthbuffer  = 0;
        m_gl_colorbuffer  = 0;
        m_gl_framebuffer  = 0;

#ifndef USE_GLSL_CONFIG
        LSGL_CompilerSetting();
#endif
    }
    
    ~Impl() {
        ReleaseBuffer_SGL(m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //ReleaseBuffer_GL(m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);
    }
    
    void LSGL_CompilerSetting()
    {
        std::string binaryPath = getBinaryDir();
#ifdef __APPLE__
        std::string binpath = "macosx64";
        std::string ccmd    = "clang++";
#elif _WIN32
        std::string binpath = "win64";
        std::string ccmd    = "g++";
#elif __linux__
        std::string binpath = "linux_x64";
        std::string ccmd    = "g++";
#elif SPARC
        std::string binpath = "sparc64";
        std::string ccmd    = "fccpx";
#endif
        std::string opt      = "-O2";
        
        std::string mesaPath = binaryPath + "glsl/bin/" + binpath + "/glsl_compiler";
        std::string compilerCmd;
        compilerCmd += binaryPath + std::string("glsl/glslc");
        compilerCmd += std::string(" --cxx=\"")      + ccmd     + std::string("\"");
        compilerCmd += std::string(" --cxxflags=\"") + opt      + std::string("\"");
        compilerCmd += std::string(" --mesacc=\"")   + mesaPath + std::string("\"");
        SetShaderCompiler_SGL(compilerCmd.c_str(), NULL);
    }

    void AddRenderObject(RenderObject* robj)
    {
        m_renderObjects.push_back(robj);
    }
    void ClearRenderObject()
    {
        m_renderObjects.clear();
    }
    
    void Render()
    {
        RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
        for (it = m_renderObjects.begin(); it != eit; ++it)
        {
            if ((*it)->GetType() == RenderObject::TYPE_CAMERA) {
                Camera* camera = static_cast<Camera*>(it->Get());
                const std::string& outfile = camera->GetOutputFile();
                BufferImageData* color = camera->GetImageBuffer();
                BufferImageData* depth = camera->GetDepthBuffer();
                
                resize(camera);
                setCurrentCamera(camera);
                renderObjects(color, depth);
                readbackImage(color);
                if (!outfile.empty()) {
                    m_imagesaver.Save(outfile.c_str(), color);
                }
            }
        }
    }
   
private:
    
    void setCurrentCamera(const Camera* camera)
    {
        m_currentCamera = camera;
        m_clearcolor = VX::Math::vec4(camera->GetClearColor());
    }
    
    const BaseBuffer* createBufferSGL(const RenderObject* robj)
    {
        const BaseBuffer* buffer = 0;
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
    
    void draw_SGL(const RenderObject* robj)
    {
        if (robj->GetType() == RenderObject::TYPE_CAMERA) {
            return;
        }
        
        const BaseBuffer* buffer = 0;
        BufferMap::const_iterator it = m_buffers_SGL.find(robj);
        if (it != m_buffers_SGL.end()) {
            buffer = it->second.Get();
        } else {
            buffer = createBufferSGL(robj);
        }

        assert(buffer);

        const float res[] = {m_width, m_height};

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

    void readbackImage(BufferImageData* color)
    {
        unsigned char * imgbuf = color->ImageBuffer()->GetBuffer();

        if (m_mode == RENDER_LSGL)
            GetColorBuffer_SGL(m_width, m_height, imgbuf);
        //else
        //	GetColorBuffer_GL(m_width, m_height, m_imgbuf);// todo nothing here!
        
#ifdef HIVE_WITH_COMPOSITOR
        int rank;
        int nnodes;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

        // @fixme { pixel format. }
        Do_234Composition(rank, nnodes, m_width, m_height, ID_RGBA32, ALPHA_BtoF, imgbuf, MPI_COMM_WORLD );
#endif

        float clearcolor_r = 0.0;
        float clearcolor_g = 0.0;
        float clearcolor_b = 0.0;

        
        // merge to bgcolor
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                const double alp = imgbuf[4*(x + y * m_width) + 3]/255.0;
                imgbuf[4*(x + y * m_width) + 0] = imgbuf[4*(x + y * m_width) + 0] * alp + 255.0*clearcolor_r * (1.0 - alp);
                imgbuf[4*(x + y * m_width) + 1] = imgbuf[4*(x + y * m_width) + 1] * alp + 255.0*clearcolor_g * (1.0 - alp);
                imgbuf[4*(x + y * m_width) + 2] = imgbuf[4*(x + y * m_width) + 2] * alp + 255.0*clearcolor_b * (1.0 - alp);
                imgbuf[4*(x + y * m_width) + 3] = 0xFF;
            }
        }

    }
    void renderObjects(BufferImageData* color, BufferImageData* depth)
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
            
            // Get buffer!
            unsigned char * imgbuf = color->ImageBuffer()->GetBuffer();
            float * depthbuf = depth->FloatImageBuffer()->GetBuffer();
            GetColorBuffer_GL(m_width, m_height, imgbuf);
            GetDepthBuffer_GL(m_width, m_height,  depthbuf);
            
            //unbindGLBuffer();
        }
 
    }
    
    void resize(Camera* camera)
    {
        BufferImageData* color = camera->GetImageBuffer();
        BufferImageData* depth = camera->GetDepthBuffer();
        const int w = camera->GetScreenWidth();
        const int h = camera->GetScreenHeight();
        
        if (m_sgl_framebuffer || m_sgl_colorbuffer || m_sgl_depthbuffer)
            ReleaseBuffer_SGL(m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //		if (m_gl_framebuffer || m_gl_colorbuffer || m_gl_depthbuffer)
        //			ReleaseBuffer_GL(m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer); // todo nothing here
        
        CreateBuffer_SGL(w, h, m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //		CreateBuffer_GL  (w, h, m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);  // todo nothing here

#ifdef HIVE_WITH_COMPOSITOR
        int rank;
        int nnodes;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nnodes);

        // Re-allocate compositor buffer for the change of screen resolution.
        // @fixme { Support various image format. Currently only RGBA 8bit allowd. }
        if (m_width != w || m_height != h) {
            if (m_width == 0 || m_height ==0) { // Assume first call of resize() function.
            } else {
                Destroy_234Composition(ID_RGBA32);
            }
            Init_234Composition (rank, nnodes, w, h, ID_RGBA32);
        }
#endif
        
        m_width  = w;
        m_height = h;
        
        color->Clear();
        depth->Clear();
        if (w != 0 && h != 0) {
            color->Create(BufferImageData::RGBA8, w, h);
            depth->Create(BufferImageData::R32F, w, h);
            float* depthbuf = depth->FloatImageBuffer()->GetBuffer();
            for(int y = 0; y < h; ++y){
                for(int x = 0; x < w; ++x){
                    depthbuf[x + y * w] = x / (float)w;
                }
            }
        }
    }

    
};


// ----------------------------------------------------

RenderCore* RenderCore::GetInstance()
{
    static RenderCore* inst = 0;
    if (inst)
        return inst;
    inst = new RenderCore();
    return inst;
}

RenderCore::RenderCore() : m_imp(new Impl()) {}
RenderCore::~RenderCore()  { delete m_imp; }

void RenderCore::AddRenderObject(RenderObject* robj)
{
    m_imp->AddRenderObject(robj);
}
void RenderCore::Render()
{
    m_imp->Render();
}
void RenderCore::ClearRenderObject()
{
    m_imp->ClearRenderObject();
}

