#include "Render/Inc/DDSTexture.h"
//
// Created by kotk4 on 04.10.2024.
//
DDSTexture::DDSTexture(ID3D11Device *dev, LPCWSTR fileName, UINT textureSlot)
{
    ID3D11Texture2D* texture;

    CreateDDSTextureFromFile(dev, fileName,
                             (ID3D11Resource**)(&texture), NULL, 0, NULL);

    D3D11_TEXTURE2D_DESC textureDesc;
    texture->GetDesc(&textureDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
    SMViewDesc.Format = textureDesc.Format;
    SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    SMViewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
    SMViewDesc.TextureCube.MostDetailedMip = 0;

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 0;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = 0;
    samplerDesc.MipLODBias = 0;

    m_textureSlot = textureSlot;

    dev->CreateShaderResourceView(texture, &SMViewDesc, &m_pShaderTexture);
    dev->CreateSamplerState(&samplerDesc, &m_pTextureSamplerState);

    texture->Release();
}

