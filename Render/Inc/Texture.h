//
// Created by kotk4 on 28.08.2024.
//

#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "d3d11shader.h"
#include "d3d11.h"

class Texture{
protected:
    ID3D11ShaderResourceView* m_pShaderTexture = NULL;
    ID3D11SamplerState* m_pTextureSamplerState = NULL;

    UINT m_textureSlot{};
public:
    Texture();
    Texture(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCSTR fileName, UINT textureSlot);
    ~Texture();

    void GetTexture(ID3D11ShaderResourceView** ppShaderTexture, ID3D11SamplerState** ppTextureSamplerState);
    UINT GetSlotNum() const;
};

#endif //RENDER_TEXTURE_H
