//
// Created by kotk4 on 06.10.2024.
//

#ifndef RENDER_DIRSHADOWMAPMATERIAL_H
#define RENDER_DIRSHADOWMAPMATERIAL_H

#include "d3d11.h"


class DirShadowMapRenderer{
private:
#define SHADOW_WIDTH 4096
#define SHADOW_HEIGHT 4096
    D3D11_VIEWPORT m_viewport;

    ID3D11Texture2D* m_shadowMap;
    ID3D11Texture2D* m_depthStencilBuffer;

    ID3D11ShaderResourceView* m_shadowMapSRV;

public:
    void GetShadowMap(ID3D11ShaderResourceView** srv);
};

#endif //RENDER_DIRSHADOWMAPMATERIAL_H
