//
// Created by kotk4 on 21.09.2024.
//

#ifndef RENDER_TESSELATIONSHADERPROGRAM_H
#define RENDER_TESSELATIONSHADERPROGRAM_H

#include "ShaderProgram.h"

class TesselationShaderProgram : public ShaderProgram{
private:
    ID3D11HullShader* pHS = NULL;
    ID3D11DomainShader* pDS = NULL;

    ID3D10Blob* HS = NULL;
    ID3D10Blob* DS = NULL;
public:
    TesselationShaderProgram(ID3D11Device* dev, ID3D11DeviceContext* devcon,
            LPCWSTR pFileName, LPCSTR vEntryPoint, LPCSTR pEntryPoint, LPCSTR hEntryPoint, LPCSTR dEntryPoint);
    ~TesselationShaderProgram();

    void ActivateShader(ID3D11DeviceContext* devcon) override;
    void VSSetConstantBuffer(ID3D11DeviceContext* devcon, UINT slot, ID3D11Buffer* pBuffer) override;

    void BindDomainTexture(ID3D11DeviceContext* devcon, Texture* texture);
    void UnbindDomainTexture(ID3D11DeviceContext* devcon, Texture* texture);
};

#endif //RENDER_TESSELATIONSHADERPROGRAM_H
