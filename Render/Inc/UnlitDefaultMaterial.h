//
// Created by kotk4 on 04.10.2024.
//

#ifndef RENDER_UNLITDEFAULTMATERIAL_H
#define RENDER_UNLITDEFAULTMATERIAL_H

#include "d3d11.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "DirectXMath.h"
#include "Camera.h"
#include "vector"
#include "Render/Inc/ConstantBuffer.h"
#include "ConstantBufferTypes.h"

using namespace DirectX;

class UnlitDefaultMaterial{
private:
    ConstantBuffer<DefaultUnlitVertexCB>* m_transformCBuffer{};

    std::vector<Texture*> m_textures;
protected:
    virtual void BindTextures();
    void UnbindTextures();

    ID3D11DeviceContext* m_devcon;
    ShaderProgram* m_shader;
public:
    UnlitDefaultMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, ShaderProgram* pShader, UINT texCount,
                    Texture** ppTextures);
    ~UnlitDefaultMaterial();

    void Render(Camera* camera, XMFLOAT4X4 modelTransform);
};

#endif //RENDER_UNLITDEFAULTMATERIAL_H
