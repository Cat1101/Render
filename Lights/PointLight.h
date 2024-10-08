//
// Created by kotk4 on 21.09.2024.
//

#ifndef RENDER_POINTLIGHT_H
#define RENDER_POINTLIGHT_H

#include "DirectXMath.h"
#include "Render/Inc/ConstantBufferTypes.h"
using namespace DirectX;

class PointLight{
private:
    PointLightShaderData lightShaderData{};
public:
    PointLight();

    void SetAttenuationCoefficients(float linear, float exponent);
    void SetPosition(XMFLOAT3 pos);
    void SetColor(XMFLOAT3 diff, XMFLOAT3 ambient, XMFLOAT3 specular);

    PointLightShaderData GetLightShaderData();
};

#endif //RENDER_POINTLIGHT_H
