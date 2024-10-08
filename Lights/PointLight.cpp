//
// Created by kotk4 on 21.09.2024.
//
#include "PointLight.h"

PointLight::PointLight() {
    lightShaderData.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

    lightShaderData.diffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
    lightShaderData.ambient = XMFLOAT3(0.1f, 0.1f, 0.1f);
    lightShaderData.specular = XMFLOAT3(0.0f, 0.0f, 0.0f);

    lightShaderData.attenuationLinC = 1.0f;
    lightShaderData.attenuationExpC = 1.0f;
}

void PointLight::SetAttenuationCoefficients(float linear, float exponent) {
    lightShaderData.attenuationLinC = linear;
    lightShaderData.attenuationExpC = exponent;
}

void PointLight::SetPosition(XMFLOAT3 pos) {
    lightShaderData.pos = pos;
}

void PointLight::SetColor(XMFLOAT3 diff, XMFLOAT3 ambient, XMFLOAT3 specular) {
    lightShaderData.diffuse = diff;
    lightShaderData.ambient = ambient;
    lightShaderData.specular = specular;
}

PointLightShaderData PointLight::GetLightShaderData() {
    return lightShaderData;
}

