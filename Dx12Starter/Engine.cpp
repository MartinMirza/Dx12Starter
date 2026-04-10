#include "Engine.h"
#include "Logger.h"

#include <dxgidebug.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

Engine::Engine(UINT width, UINT height)
    : WindowApplication(width, height), renderer(nullptr)
{
}

Engine::~Engine()
{
    delete renderer;

#if _DEBUG
    ReportLiveObjects(); 
#endif
}

void Engine::Initialize()
{
    Logger::GetInstance().Log("%s", "Engine::Initialize()\n");

    RendererSettings settings {};
    settings.isVsyncEnabled = true;
    
    renderer = new Renderer(hwnd, settings);
}

void Engine::Tick()
{
    if (renderer)
    {
        renderer->Render();
    }
}

void Engine::ReportLiveObjects()
{
    ComPtr<IDXGIDebug> dxgiDebugInterface;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebugInterface))))
    {
        dxgiDebugInterface->ReportLiveObjects(DXGI_DEBUG_ALL, static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
    }
}