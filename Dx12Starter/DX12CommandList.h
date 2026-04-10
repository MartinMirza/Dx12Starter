#pragma once

#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class DX12RenderTarget;

class DX12CommandList
{
    friend class DX12CommandQueue;
public:
    DX12CommandList(ID3D12Device* device);
    ~DX12CommandList();
    
    void TransitionTo(DX12RenderTarget* renderTarget, D3D12_RESOURCE_STATES nextState);
    void SetRenderTarget(DX12RenderTarget* renderTarget);
    void ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color);
    void Close();
    void Reset();
    
    // Friend access for CommandQueue
    ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

private:
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> commandList;
};
