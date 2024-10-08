//
// Created by kotk4 on 02.09.2024.
//

#ifndef RENDER_INPUT_H
#define RENDER_INPUT_H

#include "d3d11.h"
#include "dinput.h"

class Input{
private:
    inline static DIMOUSESTATE m_mousePrevState;
    inline static DIMOUSESTATE m_mouseState;
    inline static BYTE m_keyboardState[256];

    inline static LPDIRECTINPUT8 m_dInput;

    inline static IDirectInputDevice8* m_mouse;
    inline static IDirectInputDevice8* m_keyboard;
public:
    static HRESULT InitDirectInput(HINSTANCE hInstance, HWND hwnd);

    static void UpdateInput();

    static bool GetKey(UINT key);
    static bool GetButton(UINT button);
    static void GetCursorPos(int* x, int* y);
};

#endif //RENDER_INPUT_H
