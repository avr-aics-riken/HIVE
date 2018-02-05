/**
 * @file RenderCoreKVS.h
 * KVS RendererPlugin core
 */

#include <Core/Ref.h>
#include "RenderCoreKVS.h"
#include <RenderObject/RenderObject.h>
#include <Buffer/BufferImageData.h>
#include <Buffer/Buffer.h>
#include <RenderObject/Camera.h>
#include <Image/ImageSaver.h>

#include <stdio.h>

#include <kvs/Timer>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>

//#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace {
    ImageSaver m_imagesaver;
    kvs::Timer ktime;

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
} // namespace

class RenderCoreKVS::Impl {
private:
    typedef std::vector< RefPtr<RenderObject> > RenderObjectArray;
    RenderObjectArray m_renderObjects;
    const Camera* m_currentCamera;
    
    int m_width;
    int m_height;
    
    kvs::glut::Screen* m_screen;
    
public:
    Impl() {
        printf("RenderCoreKVS contruct\n");
        
        m_width = 512;
        m_height = 512;
        
        int argc = 1;
        char arg1[] = "KVSScreen";
        char* argv[] = { arg1 };
        kvs::glut::Application app( argc, argv );
        kvs::glut::Screen* screen = new kvs::glut::Screen( &app );
        screen->setGeometry( 0, 0, m_width, m_height);
        screen->show();
        screen->hide();
        //glewInit();
        
        m_screen = screen;
    }
    ~Impl() {
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
        printf("RenderCoreKVS Render\n");
        
        RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
        for (it = m_renderObjects.begin(); it != eit; ++it)
        {
            if ((*it)->GetType() == RenderObject::TYPE_CAMERA) {
                Camera* camera = static_cast<Camera*>(it->Get());
                const std::string& outfile = camera->GetOutputFile();
                const std::string& depth_outfile = camera->GetDepthOutputFile();
                const std::string& imagebuffer_format = camera->GetImageBufferFormat();
                BufferImageData::FORMAT colorfmt = getImageBufferFomat(outfile, imagebuffer_format);
                BufferImageData* color = camera->GetImageBuffer();
                BufferImageData* depth = camera->GetDepthBuffer();
                
                ktime.start();
                resize(camera);
                ktime.stop();
                const double resizetm = ktime.msec();
                
                ktime.start();
                setCurrentCamera(camera);
                renderObjects();
                ktime.stop();
                const double rendertm = ktime.msec();
                
                ktime.start();
                const float* clr = camera->GetClearColor();
                readbackImage(colorfmt, color, clr[0], clr[1], clr[2], clr[3]);
                readbackDepth(depth);
                ktime.stop();
                const double readbacktm = ktime.msec();
                
                
                ktime.start();
                if (!outfile.empty()) {
                    m_imagesaver.Save(outfile.c_str(), color);
                }
                if (!depth_outfile.empty()) {
                    m_imagesaver.Save(depth_outfile.c_str(), depth);
                }
                ktime.stop();
                const double savetm = ktime.msec();
                printf("[RenderPluginKVS] Resize=%.3f DrawCall=%.3f Readback=%.3f Save=%.3f\n", resizetm, rendertm, readbacktm, savetm);
                
            }
        }
    
        
    }

    void ClearBuffers()
    {
    }

    
private:
    void setCurrentCamera(const Camera* camera)
    {
        m_currentCamera = camera;
    }
    
    void readbackDepth(BufferImageData* depth)
    {
        FloatBuffer* fbuf = depth->FloatImageBuffer();
        if (fbuf) {
            float* imgbuf = fbuf->GetBuffer();
            //GetDepthBuffer_GS[m_mode](m_width, m_height, imgbuf);
        }
    }
    
    void readbackImage(BufferImageData::FORMAT format, BufferImageData* color, float clr_r, float clr_g, float clr_b, float clr_a)
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
            //GetColorBuffer_GS[m_mode](m_width, m_height, imgbuf, colorbit);
            
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
            //GetColorBuffer_GS[m_mode](m_width, m_height, reinterpret_cast<unsigned char*>(imgbuf), colorbit);
            
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
    
    void draw(RenderObject* robj) {
        if (robj->GetType() == RenderObject::TYPE_CAMERA) {
            return;
        }
        
        /*BaseBuffer* buffer = 0;
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
         */
    }
    
    /// オブジェクトのレンダリング
    void renderObjects()
    {
        //Clear_GS[m_mode](m_clearcolor[0], m_clearcolor[1], m_clearcolor[2], m_clearcolor[3]);
        
        RenderObjectArray::const_iterator it,eit = m_renderObjects.end();
        for (it = m_renderObjects.begin(); it != eit; ++it)
        {
            draw((*it));
        }
    }

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
        m_width  = w;
        m_height = h;
        
        /*if (m_gs_framebuffer || m_gs_colorbuffer || m_gs_depthbuffer)
            ReleaseBuffer_GS[m_mode](m_gs_framebuffer, m_gs_colorbuffer, m_gs_depthbuffer);
        
        const int colorbit = (colorfmt == BufferImageData::RGBA32F ? 32 : 8);
        CreateBuffer_GS[m_mode](w, h, m_gs_framebuffer, m_gs_colorbuffer, colorbit, m_gs_depthbuffer, 32);
        */
        
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

RenderCoreKVS::RenderCoreKVS() : m_imp(new Impl()) {}
RenderCoreKVS::~RenderCoreKVS() { delete m_imp; }

void RenderCoreKVS::AddRenderObject(RenderObject* robj)
{
    m_imp->AddRenderObject(robj);
}

void RenderCoreKVS::Render()
{
    m_imp->Render();
}

void RenderCoreKVS::ClearRenderObject()
{
    m_imp->ClearRenderObject();
}

void RenderCoreKVS::ClearBuffers()
{
    m_imp->ClearBuffers();
}


void RenderCoreKVS::SetProgressCallback(bool (*func)(double))
{
    // TODO
}

bool RenderCoreKVS::ClearShaderCache(const char* srcname)
{
    // TODO: DELETE THIS METHOD
    return true;
}

const char* RenderCoreKVS::GetRendererName()
{
    return "KVS";
}
