#include "DX12CommandList.h"
#include "DX12RenderTarget.h"
#include "DX12PipelineState.h"
#include "Logger.h"

DX12CommandList::DX12CommandList(ID3D12Device* device)
{
    HRESULT hr = device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&commandAllocator));
    
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create command allocator: 0x%X\n", hr);
        return;
    }
    
    hr = device->CreateCommandList(0,
        D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(),
        nullptr, IID_PPV_ARGS(&commandList));
    
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create command list: 0x%X\n", hr);
        return;
    }
    
    commandList->Close();
    Logger::GetInstance().Log("DX12CommandList created successfully\n");
}

DX12CommandList::~DX12CommandList()
{
    // ComPtr will automatically release resources
}

void DX12CommandList::TransitionTo(DX12Resource* resource, D3D12_RESOURCE_STATES nextState)
{
    if (!resource || resource->GetCurrentState() == nextState)
    {
        return;
    }
    
    Logger::GetInstance().Log("Transitioning resource state from %d to %d\n", resource->GetCurrentState(), nextState);
    
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = resource->GetResource();
    barrier.Transition.StateBefore = resource->GetCurrentState();
    barrier.Transition.StateAfter = nextState;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    commandList->ResourceBarrier(1, &barrier);
    resource->SetCurrentState(nextState);
}

void DX12CommandList::SetRenderTarget(DX12RenderTarget* renderTarget)
{
    D3D12_CPU_DESCRIPTOR_HANDLE descriptor = renderTarget->GetDescriptor();
    commandList->OMSetRenderTargets(1, &descriptor, FALSE, nullptr);
}

void DX12CommandList::ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color)
{
    commandList->ClearRenderTargetView(renderTarget->descriptorHandle, color, 0, nullptr);
}

void DX12CommandList::Close()
{
    HRESULT hr = commandList->Close();
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to close command list: 0x%X\n", hr);
    }
}

void DX12CommandList::Reset()
{
    HRESULT hr = commandAllocator->Reset();
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to reset command allocator: 0x%X\n", hr);
        return;
    }
    
    hr = commandList->Reset(commandAllocator.Get(), nullptr);
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to reset command list: 0x%X\n", hr);
    }
}

void DX12CommandList::SetPipelineState(DX12PipelineState* pipelineState)
{
    commandList->SetGraphicsRootSignature(pipelineState->rootSignature);
    commandList->SetPipelineState(pipelineState->pipelineState);
}
