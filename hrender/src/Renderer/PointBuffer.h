#pragma once
/*
    PointBuffer.h
 */

#include "BaseBuffer.h"

class PointModel;

class PointBuffer : public BaseBuffer
{
public:
    PointBuffer(RENDER_MODE mode);
    ~PointBuffer();
    
    bool Create(PointModel* point);
    void Render() const;
};

