/**
 * @file SparseVolumeModel.h
 * ボリュームモデル
 */
#ifndef _SPARSEVOLUMEMODEL_H_
#define _SPARSEVOLUMEMODEL_H_

#include <string>
#include "RenderObject.h"
#include "BufferSparseVolumeData.h"


class BufferSparseVolumeData;

/**
 * ボリュームモデル
 */
class SparseVolumeModel : public RenderObject
{
private:
    class Impl;
    Impl* m_imp;
public:
    SparseVolumeModel();
    ~SparseVolumeModel();
    bool SetShader(const std::string& shaderfile);
    bool SetClampToEdge(bool s, bool t, bool r);
    bool Create(BufferSparseVolumeData* volume);
    BufferSparseVolumeData* GetSparseVolume() const;
    const std::string& GetShader() const;

    bool GetClampToEdgeS() const;  
    bool GetClampToEdgeT() const; 
    bool GetClampToEdgeR() const; 

	virtual bool GetBox(VX::Math::vec3& bmin, VX::Math::vec3& bmax) const;
};

#endif //_SPARSEVOLUMEMODEL_H_

