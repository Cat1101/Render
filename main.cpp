#include "D3D.h"
#include "Timer.h"
#include "Input.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(220, 220, 230));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowExA(0, CLASS_NAME, "Test Window", WS_OVERLAPPEDWINDOW, 10, 10,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) return 1;

    ShowWindow(hwnd, nShowCmd);

    if(!Input::InitDirectInput(hInstance, hwnd))
    {
        MessageBox(0, "Direct Input Initialization - Failed",
                   "Error", MB_OK);
        return 1;
    }

    D3D_Init(hwnd);
    Timer frameTimer;
    frameTimer.Start();

    MSG msg = { };
    while(TRUE){
        float dt = frameTimer.GetMsElapsed();
        frameTimer.Restart();

        Input::UpdateInput();

        if(Input::GetKey(DIK_ESCAPE)){
            PostMessage(hwnd, WM_DESTROY, 0, 0);
        }

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT) break;
        }


        D3D_Render(dt);
    }

    D3D_Clean();

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}