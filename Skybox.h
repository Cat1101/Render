//
// Created by kotk4 on 04.10.2024.
//

#ifndef RENDER_SKYBOX_H
#define RENDER_SKYBOX_H

#include "Render/Inc/UnlitDefaultMaterial.h"
#include "Render/Inc/DDSTexture.h"
#include "ObjLoader.h"

class Skybox{
private:
    UnlitDefaultMaterial* m_material;
    ID3D11RasterizerState* m_rasterizerState;
    ID3D11DepthStencilState* m_depthStencilState;

    ID3D11Buffer* m_pVBuffer;
    ID3D11Buffer* m_pIBuffer;

public:
    Skybox(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCWSTR textureFileName);

    ~Skybox();

    void Render(ID3D11DeviceContext* devcon, Camera* camera);
};

#endif //RENDER_SKYBOX_H
