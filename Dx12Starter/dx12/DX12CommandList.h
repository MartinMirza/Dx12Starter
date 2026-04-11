#pragma once

#include <d3d12.h>
#include <wrl.h>

class DX12Resource;
using Microsoft::WRL::ComPtr;

class DX12RenderTarget;
class DX12PipelineState;

class DX12CommandList
{
    friend class DX12CommandQueue;
public:
    DX12CommandList(ID3D12Device* device);
    ~DX12CommandList();
    
    void TransitionTo(DX12Resource* resource, D3D12_RESOURCE_STATES nextState);
    void SetRenderTarget(DX12RenderTarget* renderTarget);
    void ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color);
    void Close();
    void Reset();
    void SetPipelineState(DX12PipelineState* pipelineState);
    
    // Friend access for CommandQueue
    ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

private:
    ComPtr<ID3D12CommandAllocator> commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> commandList;
};
