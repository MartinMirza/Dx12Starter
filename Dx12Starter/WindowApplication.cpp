#include "WindowApplication.h"
#include <iostream>

#define WINDOW_CLASS_NAME L"IntroWindow"
#define WINDOW_TITLE L"Dx12Starter"
#define DEFAULT_WINDOW_STYLE WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        {
            std::cout << " WindowProcedure - WM_DESTROY received" << '\n';

            PostQuitMessage(0);
            break;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

WindowApplication::WindowApplication(UINT inWidth, UINT inHeight)
: hwnd(nullptr), instance(nullptr), width(inWidth), height(inHeight)
{
    instance = GetModuleHandle(NULL);
    RegisterWindowClass();
    CreateWindowInstance();
}

void WindowApplication::Run()
{
    Initialize();

    MSG msg{};
    
    while (true)
    {
        bool isMessageAvailable = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

        if (isMessageAvailable)
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            DispatchMessage(&msg);
        }

        Tick();
        // Add a small sleep to prevent CPU overload
        Sleep(16); // ~60 FPS
    }
}

void WindowApplication::Initialize()
{
}

void WindowApplication::Tick()
{
}

void WindowApplication::RegisterWindowClass() const
{
    WNDCLASSEX windowClass {};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpszClassName = WINDOW_CLASS_NAME;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassEx(&windowClass);
}

void WindowApplication::CreateWindowInstance()
{
    RECT windowRectangle = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd = CreateWindowEx(NULL,
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        DEFAULT_WINDOW_STYLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRectangle.right - windowRectangle.left,
        windowRectangle.bottom - windowRectangle.top,
        NULL, NULL, instance, NULL);

    if (!hwnd)
    {
        exit(1);
    }
}
