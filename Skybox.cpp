#include "Skybox.h"
//
// Created by kotk4 on 04.10.2024.
//
Skybox::Skybox(ID3D11Device *dev, ID3D11DeviceContext *devcon, LPCWSTR textureFileName) {
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_NONE;
    dev->CreateRasterizerState(&rd, &m_rasterizerState);

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dev->CreateDepthStencilState(&dsDesc, &m_depthStencilState);

    ShaderProgram* shader = new ShaderProgram(dev, devcon, L"skybox.hlsl",
                                              "VS", "PS");

    Texture* texture = new DDSTexture(dev, textureFileName, 0);

    ObjLoader::Load(dev, devcon, L"skybox.obj", &m_pVBuffer, &m_pIBuffer);

    m_material = new UnlitDefaultMaterial(dev, devcon, shader, 1, &texture);
}

void Skybox::Render(ID3D11DeviceContext *devcon, Camera* camera) {
    XMFLOAT4X4 transform{};
    XMStoreFloat4x4(&transform, XMMatrixIdentity());

    devcon->OMSetDepthStencilState(m_depthStencilState, 0);
    devcon->RSSetState(m_rasterizerState);

    m_material->Render(camera, transform);

    UINT vSize = sizeof(VERTEX);
    UINT vOffset = 0;
    devcon->IASetVertexBuffers(0, 1, &m_pVBuffer, &vSize, &vOffset);
    devcon->IASetIndexBuffer(m_pIBuffer, DXGI_FORMAT_R32_UINT, 0);

    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    devcon->DrawIndexed(36, 0, 0);
}

Skybox::~Skybox() {
    m_pVBuffer->Release();
    m_pIBuffer->Release();
    m_rasterizerState->Release();
    m_depthStencilState->Release();

    delete m_material;
}
