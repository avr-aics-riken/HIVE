#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RenderCore.h"

#include "Commands.h"
#include <vector>
#include <map>
#include <algorithm>

#include "../RenderObject/RenderObject.h"
#include "../RenderObject/PolygonModel.h"
#include "../RenderObject/PointModel.h"
#include "../RenderObject/VolumeModel.h"
#include "../RenderObject/Camera.h"
#include "../RenderObject/PolygonModel.h"
#include "../Core/Ref.h"

#include "../Core/vxmath.h"

#include "PolygonBuffer.h"
#include "PointBuffer.h"
#include "VolumeBuffer.h"

#include "../Image/SimpleJPG.h"


namespace {
    bool tempSave(const unsigned char* rgba, int w, int h, const char* imageFile)
    {
        if (std::string(imageFile) == "")
            return false;
        if (!rgba)
            return false;
        void* jpgbuf = 0;
        int fsize = SimpleJPGSaverRGBA(&jpgbuf, w, h, rgba);
        FILE* fp = fopen(imageFile, "wb");
        fwrite(jpgbuf, 1, fsize, fp);
        fclose(fp);
        free(jpgbuf);
        printf("Save:%s\n", imageFile);
        return true;
    }
}

class RenderCore::Impl {

private:
    int m_width;
    int m_height;
    RENDER_MODE m_mode;
    
    // Framebuffers
    unsigned char* m_imgbuf;
    float* m_depthbuf;
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

    
public:
    Impl()
    {
        m_mode   = RENDER_LSGL;//RENDER_OPENGL;
        m_clearcolor = VX::Math::vec4(0,0,0,0);
        m_imgbuf   = 0;
        m_depthbuf = 0;
        m_sgl_depthbuffer = 0;
        m_sgl_colorbuffer = 0;
        m_sgl_framebuffer = 0;
        m_gl_depthbuffer  = 0;
        m_gl_colorbuffer  = 0;
        m_gl_framebuffer  = 0;
        resize(512, 512); // default size
        
        // TODO
        //std::string binaryPath = getBinaryPath();
        std::string binaryPath = "/Users/kioku/git/HIVE/hrender/";
        binaryPath += "../third_party/SURFACE/glsl/glslc";
        binaryPath += " --cxxflags=\"-O2\"";
        SetShaderCompiler_SGL(binaryPath.c_str(), NULL);

    }
    
    ~Impl() {
        ReleaseBuffer_SGL(m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //ReleaseBuffer_GL(m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);
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
                const int w = camera->GetScreenWidth();
                const int h = camera->GetScreenHeight();
                const std::string& outfile = camera->GetOutputFile();
                setCurrentCamera(camera);
                renderObjects();
                readbackImage();
                
                tempSave(m_imgbuf, w, h, outfile.c_str());
            }
        }
    }
   
private:
    
    void setCurrentCamera(const Camera* camera)
    {
        m_currentCamera = camera;
        
        const int w = camera->GetScreenWidth();
        const int h = camera->GetScreenHeight();
        resize(w, h);
    }
    
    const BaseBuffer* createBufferSGL(const RenderObject* robj)
    {
        const BaseBuffer* buffer = 0;
        if (robj->GetType() == RenderObject::TYPE_POLYGON) {
            PolygonBuffer* pbuf = new PolygonBuffer(RENDER_LSGL);
            pbuf->Create(static_cast<const PolygonModel*>(robj));
            buffer = pbuf;
        } else if (robj->GetType() == RenderObject::TYPE_POINT) {
            /*PointBuffer* pbuf = new PointBuffer(RENDER_LSGL);
            pbuf->Create(static_cast<const PointModel*>(robj));
            buffer = pbuf;*/
        } else if (robj->GetType() == RenderObject::TYPE_VOLUME) {
            /*VolumeBuffer* vbuf = new VolumeBuffer(RENDER_LSGL);
             vbuf->Create(static_cast<const VolumeModel*>(robj));
             buffer = vbuf;*/
        } else {
            // Unknow type
            printf("Unkown RenderObjet type:\n");
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

    void readbackImage()
    {
        if (m_mode == RENDER_LSGL)
            GetColorBuffer_SGL(m_width, m_height, m_imgbuf);
        //else
        //	GetColorBuffer_GL(m_width, m_height, m_imgbuf);// todo nothing here!
        
        float clearcolor_r = 0.0;
        float clearcolor_g = 0.0;
        float clearcolor_b = 0.0;
        
        // merge to bgcolor
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                const double alp = m_imgbuf[4*(x + y * m_width) + 3]/255.0;
                m_imgbuf[4*(x + y * m_width) + 0] = m_imgbuf[4*(x + y * m_width) + 0] * alp + 255.0*clearcolor_r * (1.0 - alp);
                m_imgbuf[4*(x + y * m_width) + 1] = m_imgbuf[4*(x + y * m_width) + 1] * alp + 255.0*clearcolor_g * (1.0 - alp);
                m_imgbuf[4*(x + y * m_width) + 2] = m_imgbuf[4*(x + y * m_width) + 2] * alp + 255.0*clearcolor_b * (1.0 - alp);
                m_imgbuf[4*(x + y * m_width) + 3] = 0xFF;
            }
        }

    }
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
            
            // Get buffer!
            GetColorBuffer_GL(m_width, m_height, m_imgbuf);
            GetDepthBuffer_GL(m_width, m_height, m_depthbuf);
            
            //unbindGLBuffer();
        }
 
    }
    
    void resize(int w, int h)
    {
        if (m_sgl_framebuffer || m_sgl_colorbuffer || m_sgl_depthbuffer)
            ReleaseBuffer_SGL(m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //		if (m_gl_framebuffer || m_gl_colorbuffer || m_gl_depthbuffer)
        //			ReleaseBuffer_GL(m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer); // todo nothing here
        
        CreateBuffer_SGL(w, h, m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //		CreateBuffer_GL  (w, h, m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);  // todo nothing here
        
        m_width  = w;
        m_height = h;
        
        delete [] m_imgbuf;
        m_imgbuf = 0;
        delete [] m_depthbuf;
        m_depthbuf = 0;
        
        if (w != 0 && h != 0) {
            m_imgbuf = new unsigned char[w * h * 4];
            m_depthbuf = new float[w * h];
            for(int y = 0; y < h; ++y){
                for(int x = 0; x < w; ++x){
                    m_depthbuf[x + y * w] = x / (float)w;
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

