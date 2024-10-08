//
// Created by kotk4 on 30.08.2024.
//

#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include "d3d11.h"
#include "DirectXMath.h"

using namespace DirectX;

class Camera{
private:
    XMFLOAT4X4 m_cameraMatrix{};

    XMFLOAT3 m_position{};
    XMFLOAT3 m_direction{};

    float m_fov = 0.0f;
    float m_aspectRatio = 1.0f;
    float m_nearZ = 0.0f;
    float m_farZ = 0.0f;

    void CalculateCameraMatrix();
public:
    Camera(UINT width, UINT height, float fov, float nearZ, float farZ);

    void SetTransform(XMFLOAT3 position, XMFLOAT3 viewDirection);

    XMFLOAT4X4 GetCameraMatrix();
    XMFLOAT3 GetCameraDirection();
};

#endif //RENDER_CAMERA_H
