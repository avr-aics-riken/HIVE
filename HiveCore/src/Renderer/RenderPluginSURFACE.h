/**
 * @file RenderPlugin.cpp
 * hrenderコア機能部
 */

#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Camera;
/*
#include "RenderCore.h"

#include "Commands.h"
#include "xtime.h"

#include "../RenderObject/RenderObject.h"
#include "../RenderObject/PolygonModel.h"
#include "../RenderObject/PointModel.h"
#include "../RenderObject/LineModel.h"
#include "../RenderObject/VectorModel.h"
#include "../RenderObject/TetraModel.h"
#include "../RenderObject/SolidModel.h"
#include "../RenderObject/VolumeModel.h"
#include "../RenderObject/SparseVolumeModel.h"
#include "../RenderObject/Camera.h"
#include "../RenderObject/PolygonModel.h"
#include "../Core/Ref.h"


#include "PolygonBuffer.h"
#include "PointBuffer.h"
#include "LineBuffer.h"
#include "VolumeBuffer.h"
#include "SparseVolumeBuffer.h"
#include "VectorBuffer.h"
#include "TetraBuffer.h"
#include "SolidBuffer.h"

#include "../Buffer/Buffer.h"
#include "../Buffer/BufferImageData.h"
#include "../Image/ImageSaver.h"

#include "../Core/Path.h"
#include "../Core/Perf.h"
*/

class BufferImageData;
class RenderObject;

#include "../Core/vxmath.h"


#include "RenderPlugin.h"

class RenderPluginSURFACE : public RenderPlugin {
private:
    int m_width;
    int m_height;
    int m_mode; // depcated : TODO: DELETE ME
    
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

public:
    RenderPluginSURFACE();
    ~RenderPluginSURFACE();
    
    void ClearBuffers();
    void SetParallelRendering(bool enableParallel);
    void AddRenderObject(RenderObject* robj);
    void ClearRenderObject();
    void SetProgressCallback(bool (*func)(double));
    bool GetTexture(const BufferImageData* bufimg, unsigned int& id);
    bool CreateTexture(const BufferImageData* bufimg, unsigned int& tex);
    bool DeleteTexture(const BufferImageData* bufimg);
    bool CreateProgramSrc(const char* srcname, unsigned int& prg);
    bool ClearShaderCache(const char* srcname);
    
    /// レンダリング
    void Render();
    
private:
    bool progressCallbackFunc(int progress, int y, int height);
    
    void LSGL_CompilerSetting();

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
