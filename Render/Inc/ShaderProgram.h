//
// Created by kotk4 on 28.08.2024.
//

#ifndef RENDER_SHADERPROGRAM_H
#define RENDER_SHADERPROGRAM_H

#include <list>
#include "d3d11shader.h"
#include "d3d11.h"
#include "d3dcompiler.h"
#include "Texture.h"

class ShaderProgram{
private:
    ID3D11VertexShader* pVS = NULL;
    ID3D11PixelShader* pPS = NULL;

    ID3D11InputLayout* pLayout = NULL;

    ID3D10Blob* VS = NULL;
    ID3D10Blob* PS = NULL;
public:
    ShaderProgram(ID3D11Device* dev, ID3D11DeviceContext* devcon,
                  LPCWSTR pFileName, LPCSTR vEntryPoint, LPCSTR pEntryPoint);

    ~ShaderProgram();

    void BindTexture(ID3D11DeviceContext* devcon, Texture* texture);
    void UnbindTexture(ID3D11DeviceContext* devcon, Texture* texture);

    virtual void ActivateShader(ID3D11DeviceContext* devcon);
    void SetInputLayout(ID3D11Device* dev, ID3D11DeviceContext* devcon, D3D11_INPUT_ELEMENT_DESC* ied, UINT numElements);

    virtual void VSSetConstantBuffer(ID3D11DeviceContext* devcon, UINT slot, ID3D11Buffer* pBuffer);
    void PSSetConstantBuffer(ID3D11DeviceContext* devcon, UINT slot, ID3D11Buffer* pBuffer);
};

#endif //RENDER_SHADERPROGRAM_H
