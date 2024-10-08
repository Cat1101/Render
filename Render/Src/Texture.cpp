#include "Render/Inc/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "WIC/stb_image.h"
//
// Created by kotk4 on 28.08.2024.
//
Texture::Texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCSTR fileName, UINT textureSlot) {
    ID3D11Texture2D* texture2D;
    int imgWidth, imgHeight, numColorChannels;
    byte* rawImg = stbi_load(fileName, &imgWidth, &imgHeight, &numColorChannels, 4);

    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
    textureDesc.Width = imgWidth;
    textureDesc.Height = imgHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    textureDesc.MipLevels = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    dev->CreateTexture2D(&textureDesc, NULL, &texture2D);
    devcon->UpdateSubresource(texture2D, 0, NULL,
                              rawImg, imgWidth * 4, 0);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = -1;
    dev->CreateShaderResourceView(texture2D, &srvDesc, &m_pShaderTexture);
    devcon->GenerateMips(m_pShaderTexture);

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0;

    m_textureSlot = textureSlot;

    dev->CreateSamplerState(&samplerDesc, &m_pTextureSamplerState);

    stbi_image_free(rawImg);
    texture2D->Release();
}

Texture::~Texture() {
    m_pShaderTexture->Release();
    m_pTextureSamplerState->Release();
}

void Texture::GetTexture(ID3D11ShaderResourceView** ppShaderTexture, ID3D11SamplerState** ppTextureSamplerState) {
    *ppShaderTexture = m_pShaderTexture;
    *ppTextureSamplerState = m_pTextureSamplerState;
}

UINT Texture::GetSlotNum() const {
    return m_textureSlot;
}

Texture::Texture() {
    m_pShaderTexture = NULL;
    m_pTextureSamplerState = NULL;
}
