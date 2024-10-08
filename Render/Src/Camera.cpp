#include "Render/Inc/Camera.h"
//
// Created by kotk4 on 30.08.2024.
//

Camera::Camera(UINT width, UINT height, float fov, float nearZ, float farZ) {
    m_fov = fov;
    m_aspectRatio = (float)width/(float)height;
    m_nearZ = nearZ;
    m_farZ = farZ;

    SetTransform(XMFLOAT3(0.0f, 0.0f, -0.9f), XMFLOAT3(0.0f, 0.0f, 1.0f));

    CalculateCameraMatrix();
}

void Camera::CalculateCameraMatrix() {
    XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

    XMVECTOR vPos = XMLoadFloat3(&m_position);
    XMVECTOR vDir = XMLoadFloat3(&m_direction);
    XMVECTOR vUp = XMLoadFloat3(&up);

    XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fov),
                                                   m_aspectRatio, m_nearZ, m_farZ);
    XMMATRIX viewMatrix = XMMatrixLookToLH(vPos, vDir, vUp);

    XMStoreFloat4x4(&m_cameraMatrix, XMMatrixMultiply(viewMatrix, projMatrix));
}

XMFLOAT4X4 Camera::GetCameraMatrix() {
    return m_cameraMatrix;
}

void Camera::SetTransform(XMFLOAT3 position, XMFLOAT3 viewDirection) {
    m_position = position;
    m_direction = viewDirection;

    CalculateCameraMatrix();
}

XMFLOAT3 Camera::GetCameraDirection() {
    return m_direction;
}

