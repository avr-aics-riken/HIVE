/**
 * @file RenderPlugin.h
 * Renderer Interface
 */

#pragma once

#include "../Core/Ref.h"
#include "../Core/vxmath.h"
#include "BaseBuffer.h"

class RenderPlugin {

private:
    int m_width;
    int m_height;
    RENDER_MODE m_mode; // depcated : TODO: DELETE ME
    
    // Framebuffers
    unsigned int m_gs_framebuffer, m_gs_colorbuffer, m_gs_depthbuffer;
    VX::Math::vec4 m_clearcolor;
    
    const Camera* m_currentCamera;
    
    // Rendering nodes
    typedef std::vector<RefPtr<RenderObject> > RenderObjectArray;
    RenderObjectArray m_renderObjects;
    
    // Device caches
    typedef std::map<const std::string, unsigned int> ShaderCache;
    typedef std::map<const BufferImageData*, unsigned int> TextureCache;
    typedef std::map<const RenderObject*, RefPtr<BaseBuffer> > BufferMap;
    BufferMap m_buffers;
    TextureCache m_textureCache;
    ShaderCache  m_shaderCache;

#ifdef HIVE_WITH_COMPOSITOR
	int  m_compPixelType;
	bool m_compInitialized;
#endif
    
    double m_renderTimeout;
    double m_oldCallbackTime;
    bool (*m_progressCallback)(double);
    static bool progressCallbackFunc_(int progress, int y, int height, void* ptr) {
        return static_cast<RenderPlugin*>(ptr)->progressCallbackFunc(progress, y, height);
    }
    
    bool progressCallbackFunc(int progress, int y, int height);
    
    static bool defaultProgressCallbackFunc(double progress) {
        printf("[Rendering] %3d%%\n", static_cast<int>(progress));
        return true;
    }
    
    
public:
    /// コンストラクタ
    RenderPlugin(RENDER_MODE mode);
    
    /// デストラクタ
    ~RenderPlugin();

    // TODO: DELETE ME!!!! (use only BaseBuffer)
    RENDER_MODE GetRenderMode() 
    {
        return m_mode;
    }

    /// LSGLコンパイラセッティング
    void LSGL_CompilerSetting();
    
    /// バッファのクリア
    void ClearBuffers();
    
    void SetParallelRendering(bool enableParallel);

    /// レンダーオブジェクトの追加
    /// @param robj レンダーオブジェクト
    void AddRenderObject(RenderObject* robj);
    
    /// レンダーオブジェクトのクリア
    void ClearRenderObject();
    
    /// プログレスコールバックの設定
    void SetProgressCallback(bool (*func)(double));
 
    bool GetTexture(const BufferImageData* bufimg, unsigned int& id);
    
    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex);

    bool DeleteTexture(const BufferImageData* bufimg);
    
    bool CreateProgramSrc(const char* srcname, unsigned int& prg);
    
    bool ClearShaderCache(const char* srcname);
    
    /// レンダリング
    void Render();
   
private:
    
    /// カレントカメラのセット
    /// @param camera カメラ
    void setCurrentCamera(const Camera* camera);

    /// SGLバッファの作成
    /// @param robj レンダーオブジェクト
    BaseBuffer* createBuffer(const RenderObject* robj);
    /// SGLで描画

    /// @param robj レンダーオブジェクト
    void draw(const RenderObject* robj);

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
    
};