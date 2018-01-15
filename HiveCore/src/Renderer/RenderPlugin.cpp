#include "RenderPlugin.h"


#include <RenderObject/RenderObject.h>
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

#include "PolygonBuffer.h"
#include "PointBuffer.h"
#include "LineBuffer.h"
#include "VolumeBuffer.h"
#include "SparseVolumeBuffer.h"
#include "VectorBuffer.h"
#include "TetraBuffer.h"
#include "SolidBuffer.h"


/// SGLバッファの作成
/// @param robj レンダーオブジェクト
BaseBuffer* RenderPlugin::createBuffer(const RenderObject* robj)
{
    RenderPlugin* render = this;
    BaseBuffer* buffer = 0;
    if (robj->GetType() == RenderObject::TYPE_POLYGON) {
        PolygonBuffer* pbuf = new PolygonBuffer(render);
        pbuf->Create(static_cast<const PolygonModel*>(robj));
        buffer = pbuf;
    } else if (robj->GetType() == RenderObject::TYPE_POINT) {
        PointBuffer* pbuf = new PointBuffer(render);
        pbuf->Create(static_cast<const PointModel*>(robj));
        buffer = pbuf;
    } else if (robj->GetType() == RenderObject::TYPE_LINE) {
        LineBuffer* lbuf = new LineBuffer(render);
        lbuf->Create(static_cast<const LineModel*>(robj));
        buffer = lbuf;
    } else if (robj->GetType() == RenderObject::TYPE_VOLUME) {
         VolumeBuffer* vbuf = new VolumeBuffer(render);
         vbuf->Create(static_cast<const VolumeModel*>(robj));
         buffer = vbuf;
    } else if (robj->GetType() == RenderObject::TYPE_SPARSEVOLUME) {
         SparseVolumeBuffer* vbuf = new SparseVolumeBuffer(render);
         vbuf->Create(static_cast<const SparseVolumeModel*>(robj));
         buffer = vbuf;
    } else if (robj->GetType() == RenderObject::TYPE_TETRA) {
        TetraBuffer* tbuf = new TetraBuffer(render);
        tbuf->Create(static_cast<const TetraModel*>(robj));
        buffer = tbuf;
    } else if (robj->GetType() == RenderObject::TYPE_SOLID) {
        SolidBuffer* tbuf = new SolidBuffer(render);
        tbuf->Create(static_cast<const SolidModel*>(robj));
        buffer = tbuf;
    } else if (robj->GetType() == RenderObject::TYPE_VECTOR) {
         VectorBuffer* vbuf = new VectorBuffer(render);
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
void RenderPlugin::draw(const RenderObject* robj)
{
    if (robj->GetType() == RenderObject::TYPE_CAMERA) {
        return;
    }
    
    BaseBuffer* buffer = 0;
    BufferMap::const_iterator it = m_buffers.find(robj);
    if (it != m_buffers.end()) {
        buffer = it->second.Get();
    } else {
        BaseBuffer* buf = createBuffer(robj);
        m_buffers[robj] = buf;
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