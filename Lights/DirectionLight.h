//
// Created by kotk4 on 13.09.2024.
//

#ifndef RENDER_DIRECTIONLIGHT_H
#define RENDER_DIRECTIONLIGHT_H

#include "DirectXMath.h"
#include "Render/Inc/ConstantBufferTypes.h"
using namespace DirectX;

class DirectionLight{
private:
    DirectionLightShaderData lightShaderData{};

    XMFLOAT4X4 m_viewMatrix{};
    XMFLOAT4X4 m_projectionMatrix{};

    void GenerateViewMatrix();
    void GenerateProjectionMatrix();
public:
    DirectionLight();

    void SetDirection(XMFLOAT3 dir);
    void SetColor(XMFLOAT3 diff, XMFLOAT3 ambient, XMFLOAT3 specular);

    DirectionLightShaderData GetLightShaderData();

    XMFLOAT4X4 GetViewMatrix();
    XMFLOAT4X4 GetProjectionMatrix();
};

#endif //RENDER_DIRECTIONLIGHT_H
