#include "Render/Inc/TesselationDefaultLitMaterial.h"
//
// Created by kotk4 on 22.09.2024.
//
TesselationDefaultLitMaterial::TesselationDefaultLitMaterial(ID3D11Device *dev, ID3D11DeviceContext *devcon,
                                                             TesselationShaderProgram *pShader, UINT texCount,
                                                             Texture **ppTextures, UINT domainTexCount,
                                                             Texture** ppDomainTextures) :
                                                             DefaultMaterial(dev, devcon, pShader, texCount, ppTextures)
{
    for(int i = 0; i < domainTexCount; i++){
        m_domainTextures.push_back(*ppDomainTextures++);
    }
}

TesselationDefaultLitMaterial::~TesselationDefaultLitMaterial() {
    UnbindDomainTextures();

    for(Texture* texture : m_domainTextures){
        delete texture;
    }
    m_domainTextures.clear();
}

void TesselationDefaultLitMaterial::BindTextures() {
    DefaultMaterial::BindTextures();

    auto* tessShader = dynamic_cast<TesselationShaderProgram*>(m_shader);
    if(tessShader){
        for(Texture* texture : m_domainTextures){
            tessShader->BindDomainTexture(m_devcon, texture);
        }
    }
}

void TesselationDefaultLitMaterial::UnbindDomainTextures() {
    auto* tessShader = dynamic_cast<TesselationShaderProgram*>(m_shader);
    if(tessShader){
        for(Texture* texture : m_domainTextures){
            tessShader->UnbindDomainTexture(m_devcon, texture);
        }
    }
}
