#include "Game/Inc/FirstPlayerController.h"
//
// Created by kotk4 on 02.09.2024.
//

FirstPlayerController::FirstPlayerController(Camera* camera) {
    m_camera = camera;

    m_position = XMFLOAT3(0.0f, 0.0f, -1.0f);
    m_direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

void FirstPlayerController::Update(float dt) {
    int mouseDeltaX, mouseDeltaY;

    XMVECTOR translateXmVec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR directionXmVec = XMLoadFloat3(&m_direction);
    XMVECTOR directionRightXmVec = XMVectorNegate(XMVector3Cross(directionXmVec,
                                                                 XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));

    Input::GetCursorPos(&mouseDeltaX, &mouseDeltaY);
    if(Input::GetKey(DIK_W)){
        translateXmVec = XMVectorAdd(translateXmVec, directionXmVec);
    }
    if(Input::GetKey(DIK_S)){
        translateXmVec = XMVectorAdd(translateXmVec, XMVectorNegate(directionXmVec));
    }
    if(Input::GetKey(DIK_A)){
        translateXmVec = XMVectorAdd(translateXmVec, XMVectorNegate(directionRightXmVec));
    }
    if(Input::GetKey(DIK_D)){
        translateXmVec = XMVectorAdd(translateXmVec, directionRightXmVec);
    }

    XMVECTOR quaternion = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
                                                   (float)mouseDeltaX * dt * 0.07f);
    directionXmVec = XMVector3Rotate(directionXmVec, quaternion);

    quaternion = XMQuaternionRotationAxis(directionRightXmVec,
                                          (float)mouseDeltaY * dt * 0.07f);
    directionXmVec = XMVector3Rotate(directionXmVec, quaternion);

    float angleY = XMVectorGetY(XMVector3AngleBetweenVectors(directionXmVec,
                                                XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
    angleY -= XM_PIDIV2;

    if(std::abs(angleY) <= XMConvertToRadians(40.0f))
    {
        XMStoreFloat3(&m_direction, directionXmVec);
    }

    translateXmVec = XMVectorScale(XMVector3Normalize(translateXmVec),m_speed * dt);
    XMStoreFloat3(&m_position, XMVectorAdd(translateXmVec, XMLoadFloat3(&m_position)));
    m_camera->SetTransform(m_position, m_direction);
}
