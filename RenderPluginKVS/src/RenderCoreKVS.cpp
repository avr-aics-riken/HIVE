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

// OpenGL header
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif


#include <kvs/Timer>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Camera>
#include <kvs/StochasticRenderingCompositor>


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
    
    //------
    
    void KVSCreateBuffer(int w, int h, unsigned int& framebuffer, unsigned int& colorRenderbuffer, int colorbit, unsigned int& depthRenderbuffer, int depthbit)
    {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        // create color renderbuffer and attach
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        if (colorbit == 8) {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h);
        } else if (colorbit == 32) {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F_ARB, w, h);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h);
            fprintf(stderr, "Error: Invalid color buffer format [colorbit = %d]\n use 8 or 32\n", colorbit);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        // create depth renderbuffer and attach
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        if (depthbit == 16) {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
        } else if (depthbit == 32) {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
            fprintf(stderr, "Error: Invalid depth buffer format [depthbit = %d]\n use 16 or 32\n", depthbit);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, w, h);
    }
    
    void KVSReleaseBuffer(unsigned int framebuffer, unsigned int colorRenderbuffer, unsigned int depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        glDeleteFramebuffers(1, &framebuffer);
    }
    
    void KVSGetColorBuffer(int w, int h, unsigned char* imgbuf, int colorbit)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glPixelStorei(GL_PACK_ALIGNMENT,1);
        if (colorbit == 8) {
            glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, imgbuf);
        } else if (colorbit == 32) {
            glReadPixels(0, 0, w, h, GL_RGBA, GL_FLOAT, imgbuf);
        } else {
            fprintf(stderr, "Error: Invalid color buffer format [colorbit = %d]\n use 8 or 32\n", colorbit);
        }
    }
    
    void KVSGetDepthBuffer(int w, int h, float farval, float nearval, float* depthbuf)
    {
        glFinish();
        glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depthbuf);
        glFinish();
        for (int y = 0; y < h; ++y){
            for (int x = 0; x < w; ++x){
                const float d = depthbuf[x + y * w];
                const float f = farval;
                const float n = nearval;
                depthbuf[x + y * w] = -(f * n / (d * (f - n) - f));
            }
        }
    }
    
    void KVSClear(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
} // namespace

class RenderCoreKVS::Impl {
private:
    typedef std::vector< RefPtr<RenderObject> > RenderObjectArray;
    RenderObjectArray m_renderObjects;
    
    int m_width;
    int m_height;
    float m_far;
    float m_near;
    
    unsigned int m_framebuffer, m_colorbuffer, m_depthbuffer;
    float m_clearcolor[4];
    
    kvs::glut::Screen* m_screen;
    kvs::StochasticRenderingCompositor* m_compositor;
    
public:
#ifndef __APPLE__
    int glInit() {
        GLenum e = glewInit();
        std::cout << "glewInit = " << e << std::endl;
        
        std::cout << "OpenGL " << GLVersion.major << "," << GLVersion.minor << std::endl;
        
        if (GLVersion.major < 2) {
            std::cout << "Your system doesn't support OpenGL >= 2!" << std::endl;
            return -1;
        }
        std::cout << "GL=" << glGetString(GL_VERSION) << ", GLSL=" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        return 0;
    }
#else
    int glInit() {
        std::cout << "OpenGL ver. " <<glGetString(GL_VERSION) << ", GLSL=" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        return 0;
    }
#endif
    
    Impl() {
        printf("RenderCoreKVS contruct\n");
        
        m_width = 2048;
        m_height = 2048;
        
        m_framebuffer = 0;
        m_colorbuffer = 0;
        m_depthbuffer = 0;
        
        m_clearcolor[0] = 0.0f;
        m_clearcolor[1] = 0.0f;
        m_clearcolor[2] = 0.0f;
        m_clearcolor[3] = 0.0f;
        
        m_far = 500.0;
        m_near = 0.1;
        
        int argc = 1;
        char arg1[] = "KVSScreen";
        char* argv[] = { arg1 };
        kvs::glut::Application app( argc, argv );
        kvs::glut::Screen* screen = new kvs::glut::Screen( &app );
        screen->setGeometry( 0, 0, m_width, m_height);
        screen->show();
        screen->hide();
        glInit();
        
        m_screen = screen;
        m_compositor = new kvs::StochasticRenderingCompositor( m_screen->scene() );
        if ( !m_compositor )
        {
            printf( "Cannot creat a stocastic rendering compositor.\n");
        }
        m_screen->setEvent( m_compositor );
    }
    ~Impl() {
        delete m_compositor;
        delete m_screen;
    }

    void AddRenderObject(RenderObject* robj)
    {
        m_renderObjects.push_back(robj);
    }
    void ClearRenderObject()
    {
        m_renderObjects.clear();
    }
    
    std::vector<std::pair<int, int> > m_regstered;
    
    bool KVSScreenRegister( void * obj, void* renderer )
    {
        if ( !m_screen ) { return false; }
        printf( "render called\n" );
        if ( obj && renderer ) {
            kvs::ObjectBase* object = reinterpret_cast<kvs::ObjectBase*>( obj );
            kvs::RendererBase* renderBase = reinterpret_cast<kvs::RendererBase*>( renderer );
            if ( object && renderBase ) {
                std::pair<int,int> result = m_screen->registerObject( object, renderBase );
                printf( "Register success, %d, %d \n", result.first, result.second );
                
                m_regstered.push_back( result );
                
                return true;
            }
        }
        return false;
    }
    
    bool KVSScreenClear()
    {
        if ( !m_screen ) { return false; }
        std::vector<std::pair<int, int> >::iterator it, eit = m_regstered.end();
        for (it = m_regstered.begin(); it != eit; ++it) {
            m_screen->scene()->removeObject( it->first, false, false );
        }
        m_regstered.clear();
        m_screen->reset();
        return true;
    }
    
    int render(LuaTable args, LuaTable callback)
    {
        printf("KVS render\n");
        const std::vector<LuaTable>& robjs = args.GetTable();
        printf("arg num = %d\n", static_cast<int>(robjs.size()));

        
        std::string ttypes[] = {
            "TYPE_INVALID",
            "TYPE_NUMBER",
            "TYPE_USERDATA",
            "TYPE_STRING",
            "TYPE_ARRAY",
            "TYPE_MAP",
            "TYPE_FUNCTION"
        };
        for (int i = 0; i < robjs.size(); ++i) {
            LuaTable::VALUE_TYPE vt = robjs[i].GetType();
            printf("arg[%d] = %s\n", i, ttypes[vt].c_str());
            
            // set RenderObject
            const int TYPE_USERDATA = 2;
            const int TYPE_MAP = 5;
            if (vt == TYPE_USERDATA) {
                LuaRefPtr<RenderObject>* ro = robjs[i].GetUserData<RenderObject>();
                if (!ro)
                    continue;
                AddRenderObject(*ro);
            }
            if (vt == TYPE_MAP) {
                const std::map<std::string, LuaTable>& mp = robjs[i].GetMap();
                LuaTable kobj = mp.at(std::string("object"));
                LuaTable renderer = mp.at(std::string("renderer"));
                void* objptr = kobj.GetUserData();
                void* rendptr = renderer.GetUserData();
                KVSScreenRegister(objptr, rendptr);
            }
        }
        
        m_compositor->enableLODControl();
        m_compositor->setRepetitionLevel( 50 );
        
        Render();
        
        KVSScreenClear();
        ClearRenderObject();
        return 1;
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
                
                // KVS Rendering
                m_screen->scene()->reset();

                const VX::Math::vec3 pos = camera->GetPosition();
                const VX::Math::vec3 tar = camera->GetTarget();
                const VX::Math::vec3 upd = camera->GetUp();
                
                kvs::Camera* cam = m_screen->scene()->camera();
                const kvs::Vector3<float> kpos(pos.x,pos.y,pos.z);
                const kvs::Vector3<float> ktar(tar.x,tar.y,tar.z);
                const kvs::Vector3<float> kupd(upd.x,upd.y,upd.z);
                cam->setPosition(kpos, ktar, kupd);
                cam->setFieldOfView(camera->GetFov());
                
                
                m_screen->resizeEvent(m_width, m_height);
                m_compositor->resizeEvent(m_width, m_height);
                
                m_screen->hide();
                
                const float* clr = camera->GetClearColor();
                printf("pos = (%.3f,%.3f,%.3f) tar = (%.3f,%.3f,%.3f)\n", pos.x, pos.y, pos.z, tar.x, tar.y, tar.z);
                printf("clearcolor = (%.3f,%.3f,%.3f,%.3f)\n", clr[0], clr[1], clr[2], clr[3]);
                
                ///glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
                glViewport(0, 0, m_width, m_height);
                
                m_screen->setBackgroundColor(kvs::RGBAColor(clr[0]*255, clr[1]*255, clr[2]*255, clr[3]));
                
                for (int i = 0; i < 3; ++i) {
                    m_screen->paintEvent(); // force redraw
                }
                glFlush();
                glFinish();
                
                
                ktime.stop();
                const double rendertm = ktime.msec();
                
                ktime.start();
                
                //glReadBuffer(GL_COLOR_ATTACHMENT0);
                readbackImage(colorfmt, color, clr[0], clr[1], clr[2], clr[3]);
                readbackDepth(depth);
                
                //glBindFramebuffer(GL_FRAMEBUFFER, 0);
                
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
    
    void readbackDepth(BufferImageData* depth)
    {
        FloatBuffer* fbuf = depth->FloatImageBuffer();
        if (fbuf) {
            float* imgbuf = fbuf->GetBuffer();
            KVSGetDepthBuffer(m_width, m_height, m_far, m_near, imgbuf);
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
            
            KVSGetColorBuffer(m_width, m_height, imgbuf, colorbit);
            
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
            KVSGetColorBuffer(m_width, m_height, reinterpret_cast<unsigned char*>(imgbuf), colorbit);
            
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
        
        if (m_framebuffer || m_colorbuffer || m_depthbuffer)
            KVSReleaseBuffer(m_framebuffer, m_colorbuffer, m_depthbuffer);
        
        const int colorbit = (colorfmt == BufferImageData::RGBA32F ? 32 : 8);
        KVSCreateBuffer(w, h, m_framebuffer, m_colorbuffer, colorbit, m_depthbuffer, 32);
        
        
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

int RenderCoreKVS::render(LuaTable args, LuaTable callback)
{
    return m_imp->render(args, callback);
}
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
