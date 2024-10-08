//
// Created by kotk4 on 27.08.2024.
//

#ifndef RENDER_D3D_H
#define RENDER_D3D_H

#include "windows.h"

#define SCREEN_WIDTH 1300
#define SCREEN_HEIGHT 740

void D3D_Init(HWND hwnd);
void D3D_Clean();

void D3D_Render(float dt);

#endif //RENDER_D3D_H
