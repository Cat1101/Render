#include "Render/Inc/ShaderProgram.h"
//
// Created by kotk4 on 28.08.2024.
//

ShaderProgram::ShaderProgram(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCWSTR pFileName,
                             LPCSTR vEntryPoint, LPCSTR pEntryPoint)
{
    HRESULT h;
    h = D3DCompileFromFile(pFileName, NULL, NULL, vEntryPoint,
                       "vs_5_0", 0, 0, &VS, NULL);
    D3DCompileFromFile(pFileName, NULL, NULL, pEntryPoint,
                       "ps_5_0", 0, 0, &PS, NULL);

    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(),
                            NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(),
                           NULL, &pPS);
}

ShaderProgram::~ShaderProgram()
{
    VS->Release();
    PS->Release();

    pVS->Release();
    pPS->Release();
}

void ShaderProgram::ActivateShader(ID3D11DeviceContext* devcon)
{
    devcon->IASetInputLayout(pLayout);

    devcon->HSSetShader(NULL, NULL, 0);
    devcon->DSSetShader(NULL, NULL, 0);
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);
}

void ShaderProgram::SetInputLayout(ID3D11Device* dev, ID3D11DeviceContext* devcon, D3D11_INPUT_ELEMENT_DESC* ied, UINT numElements)
{
    dev->CreateInputLayout(ied, numElements, VS->GetBufferPointer(),
                           VS->GetBufferSize(), &pLayout);
}

void ShaderProgram::BindTexture(ID3D11DeviceContext* devcon, Texture* texture) {
    ID3D11SamplerState* shaderSampler;
    ID3D11ShaderResourceView* shaderResource;
    UINT slot = texture->GetSlotNum();

    texture->GetTexture(&shaderResource, &shaderSampler);

    devcon->PSSetShaderResources(slot, 1, &shaderResource);
    devcon->PSSetSamplers(slot, 1, &shaderSampler);
}

void ShaderProgram::UnbindTexture(ID3D11DeviceContext* devcon, Texture* texture) {
    ID3D11SamplerState* shaderSampler = NULL;
    ID3D11ShaderResourceView* shaderResource = NULL;
    UINT slot = texture->GetSlotNum();

    devcon->PSSetShaderResources(slot, 1, &shaderResource);
    devcon->PSSetSamplers(slot, 1, &shaderSampler);
}

void ShaderProgram::VSSetConstantBuffer(ID3D11DeviceContext *devcon, UINT slot, ID3D11Buffer *pBuffer) {
    devcon->VSSetConstantBuffers(slot, 1, &pBuffer);
}

void ShaderProgram::PSSetConstantBuffer(ID3D11DeviceContext *devcon, UINT slot, ID3D11Buffer *pBuffer) {
    devcon->PSSetConstantBuffers(slot, 1, &pBuffer);
}

