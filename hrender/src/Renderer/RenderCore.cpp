#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RenderCore.h"

#include "Commands.h"
#include <vector>

#include "../RenderObject/RenderObject.h"
#include "../Core/Ref.h"

#include "../Image/SimpleJPG.h"

namespace {
    bool tempSave(const unsigned char* rgba, int w, int h, const char* imageFile)
    {
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
    unsigned char* m_imgbuf;
    float* m_depthbuf;
    unsigned int m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer;
    unsigned int m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer;

    typedef std::vector<RefPtr<RenderObject> > RenderObjectArray;
    RenderObjectArray m_renderObjects;
public:
    Impl()
    {
        m_mode   = RENDER_LSGL;//RENDER_OPENGL;
        m_imgbuf = 0;
        m_depthbuf = 0;
        m_sgl_depthbuffer = 0;
        m_sgl_colorbuffer = 0;
        m_sgl_framebuffer = 0;
        m_gl_depthbuffer  = 0;
        m_gl_colorbuffer  = 0;
        m_gl_framebuffer  = 0;
        resize(512, 512);
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
                setupCamera((*it));
                renderObjects();
                readbackImage();
                
                tempSave(m_imgbuf, m_width, m_height, "image.jpg"); // TEST
            }
        }
    }
   
private:
    
    void setupCamera(RenderObject* camera)
    {
        
        
    }
    
    void drawObj_SGL(RenderObject* camera)
    {
        
    }

    void drawObj_GL(RenderObject* camera)
    {
        
    }

    void readbackImage()
    {
        if (m_mode == RENDER_LSGL)
            GetColorBuffer_SGL(m_width, m_height, m_imgbuf);
        //else
        //	GetColorBuffer_GL(m_width, m_height, m_imgbuf);// todo nothing here!
        
        float clearcolor_r = 0.0;
        float clearcolor_g = 1.0;
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
        
        const float res[] = {static_cast<float>(m_width), static_cast<float>(m_height)};
        const float bgcolor[] = {0,0.0,0,0.0};//{m_clearcolor_r,m_clearcolor_g,m_clearcolor_b,m_clearcolor_a};
        if (m_mode == RENDER_LSGL) {
            //Clear_SGL(m_clearcolor_r,m_clearcolor_g,m_clearcolor_b,m_clearcolor_a);
            Clear_SGL(bgcolor[0], bgcolor[1], bgcolor[2], bgcolor[3]);
            
            //SampleCoverage_SGL(m_fsaa, 0);
            //PixelStep_SGL(m_pixelstep);
            
            RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
            for (it = m_renderObjects.begin(); it != eit; ++it)
            {
                if ((*it)->GetType() != RenderObject::TYPE_POLYGON
                &&  (*it)->GetType() != RenderObject::TYPE_POINT
                &&  (*it)->GetType() != RenderObject::TYPE_VOLUME)
                    continue;
                
                drawObj_SGL((*it));
                /*
                unsigned int prog = (*it)->GetSGLProgram();
                if (!prog)
                    continue;
                // TODO: move to Render()
                BindProgram_SGL(prog);
                SetUniform2fv_SGL(prog, "resolution", res);
                SetUniform4fv_SGL(prog, "backgroundColor", bgcolor);
                SetCamera_SGL(prog, m_eye, m_lookat, m_up, m_fov);
                //SetStereoEnvCamera_SGL(prog, m_eye, m_lookat, m_up, 20.0f, 0.03f); // fixme
                (*it)->Render(RENDER_LSGL);*/
            }
            
            //BindProgram_SGL(0); // TODO: not need to release?
        } else {
            //bindGLBuffer();
            //Clear_GL(m_clearcolor_r,m_clearcolor_g,m_clearcolor_b,m_clearcolor_a);
            Clear_GL(bgcolor[0], bgcolor[1], bgcolor[2], bgcolor[3]);
            
            RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
            for (it = m_renderObjects.begin(); it != eit; ++it)
            {
                if ((*it)->GetType() != RenderObject::TYPE_POLYGON
                &&  (*it)->GetType() != RenderObject::TYPE_POINT
                &&  (*it)->GetType() != RenderObject::TYPE_VOLUME)
                    continue;
                
                drawObj_GL((*it));
                /*
                unsigned int prog = (*it)->GetGLProgram();
                if (!prog)
                    continue;
                // TODO: move to Render()
                BindProgram_GL(prog);
                SetUniform2fv_GL(prog, "resolution", res);
                SetUniform4fv_GL(prog, "backgroundColor", bgcolor);
                
                SetCamera_GL(prog, m_eye, m_lookat, m_up, m_fov, m_width, m_height, m_near, m_far);
                (*it)->Render(RENDER_OPENGL);*/
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
        
        CreateBuffer_LSGL(w, h, m_sgl_framebuffer, m_sgl_colorbuffer, m_sgl_depthbuffer);
        //		CreateBuffer_GL  (w, h, m_gl_framebuffer, m_gl_colorbuffer, m_gl_depthbuffer);  // todo nothing here
        
        m_width  = w;
        m_height = h;
        
        delete [] m_imgbuf;
        m_imgbuf = new unsigned char[w * h * 4];
        delete [] m_depthbuf;
        m_depthbuf = new float[w * h];
        for(int y = 0; y < h; ++y){
            for(int x = 0; x < w; ++x){
                m_depthbuf[x + y * w] = x / (float)w;
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

