/**
 * @file Volren.h
 */

#ifndef _VOLREN_H
#define _VOLREN_H

#include "Ref.h"
#include "../Core/vxmath.h"
#include "../HiveCore/src/RenderObject/VolumeModel.h"
#include "../HiveCore/src/RenderObject/RenderObject.h"

class BufferImageData;

/**
 * Volren
 */
class Volren : public VolumeModel
{
public:
	Volren();
	~Volren();
    
    int SetTransferFunctionTexture(BufferImageData* tex);
    int SetTransferFunctionMin(float minVal);
    int SetTransferFunctionMax(float maxVal);
    int SetGradientTexure(BufferImageData* tex);
    
    int EnableLighting(bool enable);    
    int SetLight(float posX, float posY, float posZ, float specular);
    int SetAttenuation(float constVal, float linearVal, float quadraticVal);
    int SetAmbient(float r, float g, float b);
    int SetSpecular(float r, float g, float b);
    int SetSamples(int samples);
    int SetOffset(float x, float y, float z);
    
protected:

};

#endif //_VOLREN_H

