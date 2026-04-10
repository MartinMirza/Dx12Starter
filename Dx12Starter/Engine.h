#pragma once

#include "WindowApplication.h"
#include "Renderer.h"

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

class Engine : public WindowApplication
{
public:
    Engine(UINT width = DEFAULT_WINDOW_WIDTH, UINT height = DEFAULT_WINDOW_HEIGHT);
    ~Engine() override;
private:
    void Initialize() override;
    void Tick() override;
    static void ReportLiveObjects();
    Renderer* renderer;
};