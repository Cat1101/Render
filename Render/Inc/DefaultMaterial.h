//
// Created by kotk4 on 31.08.2024.
//

#ifndef RENDER_DEFAULTMATERIAL_H
#define RENDER_DEFAULTMATERIAL_H

#include "d3d11.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "DirectXMath.h"
#include "Camera.h"
#include "vector"
#include "Render/Inc/ConstantBuffer.h"
#include "ConstantBufferTypes.h"
#include "Lights/DirectionLight.h"
#include "Lights/PointLight.h"

using namespace DirectX;

class DefaultMaterial{
private:
    ConstantBuffer<DefaultUnlitVertexCB>* m_transformCBuffer{};
    ConstantBuffer<DefaultLightingCB>* m_lightCBuffer{};

    std::vector<Texture*> m_textures;

    DirectionLight* m_dirLightSource;
    std::vector<PointLight*> m_pointLightsSource;
protected:
    virtual void BindTextures();
    void UnbindTextures();

    ID3D11DeviceContext* m_devcon;
    ShaderProgram* m_shader;
public:
    DefaultMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, ShaderProgram* pShader, UINT texCount,
                    Texture** ppTextures);
    ~DefaultMaterial();

    void Render(Camera* camera, XMFLOAT4X4 modelTransform);

    void SetDirLightSource(DirectionLight* dirLightSource);
    void AddPointLightSource(PointLight* pointLightSource);
};

#endif //RENDER_DEFAULTMATERIAL_H
