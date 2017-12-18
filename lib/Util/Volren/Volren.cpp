/**
 * @file Volren.cpp
 */

#include "Volren.h"

class Volren::Impl
{
public:
    /// コンストラクタ
    Impl()
    {
    }

    /// デストラクタ
    ~Impl()
    {
    }
};

/// コンストラクタ
Volren::Volren() : RenderObject(TYPE_VOLUME)
{
    m_imp = new Impl();
}

/// デストラクタ
Volren::~Volren()
{
    delete m_imp;
}

int Volren::SetTransferFunctionTexture(BufferImageData* tex)
{
    SetTexture("tf_tex", tex);
}

int Volren::SetTransferFunctionMin(float minVal)
{
    SetFloat("tf_min", minVal);
}

int Volren::SetTransferFunctionMax(float maxVal)
{
    SetFloat("tf_max", maxVal);
}

int Volren::SetGradientTexure(BufferImageData* tex)
{
    SetTexture("u_gradTex", tex);
}

int Volren::EnableLighting(bool enable)
{
    SetFloat("u_enableLighting", enable ? 1.0f : 0.0f);
}

int Volren::SetLight(float posX, float posY, float posZ, float specular)
{
    SetVec4("u_light", posX, posY, posZ, specular);
}

int Volren::SetAttenuation(float constVal, float linearVal, float quadraticVal)
{
    SetVec3("u_attenuation", constVal, linearVal, quadraticVal);
}

int Volren::SetAmbient(float r, float g, float b)
{
    SetVec3("u_ambient", r, g, b);
}

int Volren::SetSpecular(float r, float g, float b)
{
    SetVec3("u_specular", r, g, b);
}

int Volren::SetSamples(int samples)
{
    SetFloat("u_samples", static_cast<float>(samples));
}

int Volren::SetOffset(float x, float y, float z)
{
    SetVec3("u_offset", x, y, z);
}