#include "Loader.h"
#include "SimpleObj.h"


// -----------------------------------------------------------------------------

OBJLoader::OBJLoader(){}
OBJLoader::~OBJLoader(){};
void OBJLoader::Clear()
{
	m_pos      = 0;
	m_normal   = 0;
    m_texcoord = 0;
	m_mat      = 0;
	m_index    = 0;
}
bool OBJLoader::Load(const char* filename){
	Clear();
	SimpleObj obj;
	bool r = obj.Load(filename);
	m_pos = new Vec3Buffer();
	m_normal = new Vec3Buffer();
	m_mat = new FloatBuffer();
	m_index = new UintBuffer();
    m_texcoord = new Vec2Buffer();
    m_pos->Create(obj.GetVertexNum());
	float* pp = m_pos->GetBuffer();
    memcpy(pp, obj.GetPositionBuffer(), sizeof(float)*3*(obj.GetVertexNum()));
	m_normal->Create(obj.GetVertexNum());
	memcpy(m_normal->GetBuffer(), obj.GetNormalBuffer(), sizeof(float)*3*obj.GetVertexNum());
    float* objuv = obj.GetUVBuffer();
    if (objuv) {
        m_texcoord->Create(obj.GetVertexNum());
        float* uv = m_texcoord->GetBuffer();
        memcpy(uv, objuv, sizeof(float)*2*(obj.GetVertexNum()));
    }
	m_mat->Create(obj.GetVertexNum());
	memset(m_mat->GetBuffer(), 0, sizeof(float)*m_mat->GetNum());
	m_index->Create(obj.GetIndexNum());
	memcpy(m_index->GetBuffer(), obj.GetIndex(), sizeof(unsigned int)*m_index->GetNum());
	return r;
}
//------------------------------------------------------------------------------


