/*
    VolumeBuffer.h
*/

#include "BaseBuffer.h"

class VolumeModel;

class VolumeBuffer : public BaseBuffer
{
public:
    VolumeBuffer(RENDER_MODE mode);
    ~VolumeBuffer();
    
    bool Create(VolumeModel* volume);
    void Render() const;
};

