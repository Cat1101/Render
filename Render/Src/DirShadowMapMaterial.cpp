#include "Render/Inc/DirShadowMapMaterial.h"
//
// Created by kotk4 on 06.10.2024.
//
void DirShadowMapRenderer::GetShadowMap(ID3D11ShaderResourceView** srv) {
    *srv = m_shadowMapSRV;
}
