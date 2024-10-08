//
// Created by kotk4 on 04.10.2024.
//

#ifndef RENDER_DDSTEXTURE_H
#define RENDER_DDSTEXTURE_H

#include "d3d11shader.h"
#include "Texture.h"
#include "DDSTextureLoader11.h"
#include "d3d11.h"

using namespace DirectX;

class DDSTexture : public Texture{
public:
    DDSTexture(ID3D11Device* dev, LPCWSTR fileName, UINT textureSlot);
};
#endif //RENDER_DDSTEXTURE_H
