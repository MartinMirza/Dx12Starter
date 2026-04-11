#pragma once

#include <d3d12.h>

class DX12Viewport
{
    friend class DX12CommandList;
public:
    DX12Viewport(UINT width, UINT height);
    ~DX12Viewport();
    
    // Accessors for viewport and scissor
    const D3D12_VIEWPORT& GetViewport() const { return viewport; }
    const D3D12_RECT& GetScissorRect() const { return scissors; }

private:
    D3D12_VIEWPORT viewport;
    D3D12_RECT scissors;
};