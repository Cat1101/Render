#include "Input.h"
//
// Created by kotk4 on 02.09.2024.
//
HRESULT Input::InitDirectInput(HINSTANCE hInstance, HWND hwnd) {
    ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));
    ZeroMemory(&m_mousePrevState, sizeof(DIMOUSESTATE));
    ZeroMemory(m_keyboardState, sizeof(m_keyboardState));

    DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
                            (void**)&m_dInput, NULL);

    m_dInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    m_dInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);

    m_mouse->SetDataFormat(&c_dfDIMouse);
    m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

    m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

    return true;
}

void Input::UpdateInput() {
    m_keyboard->Acquire();
    m_mouse->Acquire();

    m_keyboard->GetDeviceState(256, (LPVOID)&m_keyboardState);
    m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
}

bool Input::GetKey(UINT key) {
    if(m_keyboardState[key] & 0x80) return true;
    return false;
}

bool Input::GetButton(UINT button) {
    if(m_mouseState.rgbButtons[button] & 0x80) return true;
    return false;
}

void Input::GetCursorPos(int *x, int *y) {
    *x = m_mouseState.lX;
    *y = m_mouseState.lY;
}
