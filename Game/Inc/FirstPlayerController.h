//
// Created by kotk4 on 02.09.2024.
//

#ifndef RENDER_FIRSTPLAYERCONTROLLER_H
#define RENDER_FIRSTPLAYERCONTROLLER_H

#include "Render/Inc/Camera.h"
#include "d3d11.h"
#include "Input.h"
#include "DirectXMath.h"
#include <cmath>

class FirstPlayerController{
private:
    Camera* m_camera;

    XMFLOAT3 m_position;
    XMFLOAT3 m_direction;

    float m_speed = 1.8f;

public:
    FirstPlayerController(Camera* camera);

    void Update(float dt);
};

#endif //RENDER_FIRSTPLAYERCONTROLLER_H
