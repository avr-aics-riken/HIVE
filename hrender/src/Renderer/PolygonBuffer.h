#pragma once
/*
    PolygonBuffer.h
*/

#include "BaseBuffer.h"

class PolygonModel;

class PolygonBuffer : public BaseBuffer
{
public:
    PolygonBuffer(RENDER_MODE mode);
    ~PolygonBuffer();
    
    bool Create(const PolygonModel* model);
    void Render() const;
};

