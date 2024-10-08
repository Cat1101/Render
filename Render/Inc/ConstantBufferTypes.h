//
// Created by kotk4 on 31.08.2024.
//

#ifndef RENDER_CONSTANTBUFFERTYPES_H
#define RENDER_CONSTANTBUFFERTYPES_H

#include "DirectXMath.h"

using namespace DirectX;

struct DirectionLightShaderData{
    XMFLOAT3 dir;
    float pad1;
    XMFLOAT3 ambient;
    float pad2;
    XMFLOAT3 diffuse;
    float pad3;
    XMFLOAT3 specular;
    float pad4;
};

struct PointLightShaderData{
    XMFLOAT3 pos;

    float attenuationLinC;
    float attenuationExpC;

    XMFLOAT3 ambient;
    XMFLOAT3 diffuse;
    float pad1;
    XMFLOAT3 specular;
    float pad2;
};

struct SpotLightShaderData{
    XMFLOAT3 pos;
    float pad1;
    XMFLOAT3 dir;

    float innerConeAngle;
    float outerConeAngle;

    XMFLOAT3 ambient;
    XMFLOAT3 diffuse;
    float pad2;
    XMFLOAT3 specular;
    float pad3;
};

struct DefaultUnlitVertexCB{
    DefaultUnlitVertexCB(){
        XMStoreFloat4x4(&cameraMatrix, XMMatrixIdentity());
        XMStoreFloat4x4(&modelTransform, XMMatrixIdentity());
    }

    XMFLOAT4X4 cameraMatrix{};
    XMFLOAT4X4 modelTransform{};
    XMFLOAT3 viewDir{};
    float pad{};
};

struct DefaultLightingCB{
    DirectionLightShaderData dirLight{};
    PointLightShaderData pointLights[4]{};
    SpotLightShaderData spotLights[4]{};
};

#endif //RENDER_CONSTANTBUFFERTYPES_H
