#include "DX12Viewport.h"
#include "Logger.h"

DX12Viewport::DX12Viewport(UINT width, UINT height)
{
    // Initialize viewport
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    
    // Initialize scissor rectangle
    scissors.left = 0;
    scissors.top = 0;
    scissors.right = static_cast<LONG>(width);
    scissors.bottom = static_cast<LONG>(height);
    
    Logger::GetInstance().Log("DX12Viewport created (Width: %d, Height: %d)\n", width, height);
}

DX12Viewport::~DX12Viewport()
{
    Logger::GetInstance().Log("DX12Viewport destroyed\n");
}