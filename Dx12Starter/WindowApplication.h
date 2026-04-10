#pragma once

#include <Windows.h>

class WindowApplication
{
public:
    WindowApplication(UINT inWidth, UINT inHeight);
    virtual ~WindowApplication() = default;
    void Run();

protected:
    virtual void Initialize();
    virtual void Tick();
    void RegisterWindowClass() const;
    void CreateWindowInstance();
    HWND hwnd;
    HINSTANCE instance;
    LONG width;
    LONG height;
};
