#include "Render/Inc/UnlitDefaultMaterial.h"
//
// Created by kotk4 on 04.10.2024.
//

UnlitDefaultMaterial::UnlitDefaultMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, ShaderProgram* pShader,
                                 UINT texCount, Texture** ppTextures)
{
    m_shader = pShader;
    m_devcon = devcon;

    for(int i = 0; i < texCount; i++){
        m_textures.push_back(*ppTextures++);
    }

    m_transformCBuffer = new ConstantBuffer<DefaultUnlitVertexCB>(dev, devcon);

    D3D11_INPUT_ELEMENT_DESC ied[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
                    0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
                    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    m_shader->SetInputLayout(dev, devcon, ied, 2);
}

UnlitDefaultMaterial::~UnlitDefaultMaterial() {
    UnbindTextures();

    for(Texture* texture : m_textures){
        delete texture;
    }
    m_textures.clear();

    delete m_shader;
    delete m_transformCBuffer;
}

void UnlitDefaultMaterial::BindTextures() {
    for(Texture* texture : m_textures){
        m_shader->BindTexture(m_devcon, texture);
    }
}

void UnlitDefaultMaterial::UnbindTextures() {
    for(Texture* texture : m_textures){
        m_shader->UnbindTexture(m_devcon, texture);
    }
}

void UnlitDefaultMaterial::Render(Camera* camera, XMFLOAT4X4 modelTransform) {
    DefaultUnlitVertexCB defaultTransformVertexCb;
    DefaultLightingCB defaultLightingCb;

    ZeroMemory(&defaultLightingCb, sizeof(DefaultLightingCB));

    defaultTransformVertexCb.modelTransform = modelTransform;
    defaultTransformVertexCb.cameraMatrix = camera->GetCameraMatrix();
    defaultTransformVertexCb.viewDir = camera->GetCameraDirection();

    m_transformCBuffer->SetData(m_devcon, defaultTransformVertexCb);

    m_shader->ActivateShader(m_devcon);
    BindTextures();
    m_shader->VSSetConstantBuffer(m_devcon, 0, m_transformCBuffer->Get());
}