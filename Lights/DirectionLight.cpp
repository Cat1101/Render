#include "DirectionLight.h"
//
// Created by kotk4 on 13.09.2024.
//
DirectionLight::DirectionLight() {
    lightShaderData.dir = XMFLOAT3(0.0f, 0.0f, 0.0f);

    lightShaderData.diffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
    lightShaderData.ambient = XMFLOAT3(0.1f, 0.1f, 0.1f);
    lightShaderData.specular = XMFLOAT3(0.0f, 0.0f, 0.0f);

    GenerateProjectionMatrix();
}

DirectionLightShaderData DirectionLight::GetLightShaderData() {
    return lightShaderData;
}

void DirectionLight::SetDirection(XMFLOAT3 dir) {
    lightShaderData.dir = dir;

    GenerateViewMatrix();
}

void DirectionLight::SetColor(XMFLOAT3 diff, XMFLOAT3 ambient, XMFLOAT3 specular) {
    lightShaderData.diffuse = diff;
    lightShaderData.ambient = ambient;
    lightShaderData.specular = specular;
}

void DirectionLight::GenerateViewMatrix() {
    XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

    XMVECTOR vPos = XMVectorScale(XMLoadFloat3(&lightShaderData.dir), -6.0f);
    XMVECTOR vDir = XMLoadFloat3(&lightShaderData.dir);
    XMVECTOR vUp = XMLoadFloat3(&up);

    XMMATRIX viewMatrix = XMMatrixLookToLH(vPos, vDir, vUp);

    XMStoreFloat4x4(&m_viewMatrix, viewMatrix);
}

void DirectionLight::GenerateProjectionMatrix() {
    XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(XM_PIDIV2),
                                                   1.0f, 1.0, 15.0);

    XMStoreFloat4x4(&m_projectionMatrix, projMatrix);
}

XMFLOAT4X4 DirectionLight::GetViewMatrix() {
    return m_viewMatrix;
}

XMFLOAT4X4 DirectionLight::GetProjectionMatrix() {
    return m_projectionMatrix;
}
