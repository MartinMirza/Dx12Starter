#pragma once

#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class DX12CommandList;

class DX12CommandQueue
{
public:
    DX12CommandQueue();
    ~DX12CommandQueue();

    void Initialize(ID3D12Device* device);
    void Execute(const ComPtr<ID3D12CommandList>& commandList);
    void Execute(DX12CommandList* commandList);
    void Flush();
    void WaitForGpu();
    void Signal();

    const ComPtr<ID3D12CommandQueue>& GetCommandQueue() const { return commandQueue; }

private:
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<ID3D12Fence> fence;
    HANDLE fenceEvent;
    UINT64 fenceValue = 0;
};