//
// Created by kotk4 on 22.09.2024.
//

#ifndef RENDER_TESSELATIONDEFAULTLITMATERIAL_H
#define RENDER_TESSELATIONDEFAULTLITMATERIAL_H

#include "DefaultMaterial.h"
#include "Texture.h"
#include "TesselationShaderProgram.h"

class TesselationDefaultLitMaterial : public DefaultMaterial{
private:
    std::vector<Texture*> m_domainTextures;
protected:
    void BindTextures() override;
    void UnbindDomainTextures();
public:
    TesselationDefaultLitMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, TesselationShaderProgram* pShader,
                                  UINT texCount, Texture** ppTextures, UINT domainTexCount,
                                  Texture** ppDomainTextures);

    ~TesselationDefaultLitMaterial();
};

#endif //RENDER_TESSELATIONDEFAULTLITMATERIAL_H
