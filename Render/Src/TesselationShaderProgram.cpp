#include "Render/Inc/TesselationShaderProgram.h"
//
// Created by kotk4 on 21.09.2024.
//
TesselationShaderProgram::TesselationShaderProgram(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCWSTR pFileName,
                                                   LPCSTR vEntryPoint, LPCSTR pEntryPoint, LPCSTR hEntryPoint,
                                                   LPCSTR dEntryPoint)
                                                   : ShaderProgram(dev, devcon, pFileName, vEntryPoint, pEntryPoint)
{
    D3DCompileFromFile(pFileName, NULL, NULL, hEntryPoint,
                       "hs_5_0", 0, 0, &HS, NULL);
    D3DCompileFromFile(pFileName, NULL, NULL, dEntryPoint,
                       "ds_5_0", 0, 0, &DS, NULL);

    dev->CreateHullShader(HS->GetBufferPointer(), HS->GetBufferSize(),
                            NULL, &pHS);
    dev->CreateDomainShader(DS->GetBufferPointer(), DS->GetBufferSize(),
                           NULL, &pDS);
}

void TesselationShaderProgram::ActivateShader(ID3D11DeviceContext *devcon) {
    ShaderProgram::ActivateShader(devcon);

    devcon->HSSetShader(pHS, NULL, 0);
    devcon->DSSetShader(pDS, NULL, 0);
}

TesselationShaderProgram::~TesselationShaderProgram(){
    HS->Release();
    DS->Release();

    pHS->Release();
    pDS->Release();
}

void TesselationShaderProgram::VSSetConstantBuffer(ID3D11DeviceContext *devcon, UINT slot, ID3D11Buffer *pBuffer) {
    ShaderProgram::VSSetConstantBuffer(devcon, slot, pBuffer);
    devcon->DSSetConstantBuffers(slot, 1, &pBuffer);
}

void TesselationShaderProgram::BindDomainTexture(ID3D11DeviceContext *devcon, Texture *texture) {
    ID3D11SamplerState* shaderSampler;
    ID3D11ShaderResourceView* shaderResource;
    UINT slot = texture->GetSlotNum();

    texture->GetTexture(&shaderResource, &shaderSampler);

    devcon->DSSetShaderResources(slot, 1, &shaderResource);
    devcon->DSSetSamplers(slot, 1, &shaderSampler);
}

void TesselationShaderProgram::UnbindDomainTexture(ID3D11DeviceContext *devcon, Texture *texture) {
    ID3D11SamplerState* shaderSampler = NULL;
    ID3D11ShaderResourceView* shaderResource = NULL;
    UINT slot = texture->GetSlotNum();

    devcon->DSSetShaderResources(slot, 1, &shaderResource);
    devcon->DSSetSamplers(slot, 1, &shaderSampler);
}