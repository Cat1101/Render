#include "Render/Inc/DefaultMaterial.h"
//
// Created by kotk4 on 31.08.2024.
//

DefaultMaterial::DefaultMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, ShaderProgram* pShader,
                                 UINT texCount, Texture** ppTextures)
{
    m_shader = pShader;
    m_devcon = devcon;

    for(int i = 0; i < texCount; i++){
        m_textures.push_back(*ppTextures++);
    }

    m_transformCBuffer = new ConstantBuffer<DefaultUnlitVertexCB>(dev, devcon);
    m_lightCBuffer = new ConstantBuffer<DefaultLightingCB>(dev, devcon);

    D3D11_INPUT_ELEMENT_DESC ied[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
                    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TAN", 0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BITAN", 0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    m_shader->SetInputLayout(dev, devcon, ied, 5);

    m_dirLightSource = NULL;
}

DefaultMaterial::~DefaultMaterial() {
    UnbindTextures();

    for(Texture* texture : m_textures){
        delete texture;
    }
    m_textures.clear();

    delete m_shader;
    delete m_transformCBuffer;
    delete m_lightCBuffer;
}

void DefaultMaterial::BindTextures() {
    for(Texture* texture : m_textures){
        m_shader->BindTexture(m_devcon, texture);
    }
}

void DefaultMaterial::UnbindTextures() {
    for(Texture* texture : m_textures){
        m_shader->UnbindTexture(m_devcon, texture);
    }
}

void DefaultMaterial::Render(Camera* camera, XMFLOAT4X4 modelTransform) {
    DefaultUnlitVertexCB defaultUnlitVertexCb;
    DefaultLightingCB defaultLightingCb;

    ZeroMemory(&defaultLightingCb, sizeof(DefaultLightingCB));

    defaultUnlitVertexCb.modelTransform = modelTransform;
    defaultUnlitVertexCb.cameraMatrix = camera->GetCameraMatrix();
    defaultUnlitVertexCb.viewDir = camera->GetCameraDirection();

    if(m_dirLightSource != NULL){
        defaultLightingCb.dirLight = m_dirLightSource->GetLightShaderData();
    }

    for(int i = 0; i < m_pointLightsSource.size(); i++){
        defaultLightingCb.pointLights[i] = m_pointLightsSource[i]->GetLightShaderData();
    }

    m_transformCBuffer->SetData(m_devcon, defaultUnlitVertexCb);
    m_lightCBuffer->SetData(m_devcon, defaultLightingCb);

    m_shader->ActivateShader(m_devcon);
    BindTextures();
    m_shader->VSSetConstantBuffer(m_devcon, 0, m_transformCBuffer->Get());
    m_shader->PSSetConstantBuffer(m_devcon, 1, m_lightCBuffer->Get());
}

void DefaultMaterial::SetDirLightSource(DirectionLight* dirLightSource) {
    m_dirLightSource = dirLightSource;
}

void DefaultMaterial::AddPointLightSource(PointLight *pointLightSource) {
    m_pointLightsSource.push_back(pointLightSource);
}
