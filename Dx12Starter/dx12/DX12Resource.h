#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Logger.h"

using Microsoft::WRL::ComPtr;

class DX12Resource
{
public:
    DX12Resource();
    DX12Resource(ID3D12Device* device, UINT64 bufferSize);
    ~DX12Resource();
    
    void CreateBuffer(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_FLAGS flags,
                     D3D12_RESOURCE_STATES initialState, const D3D12_HEAP_PROPERTIES* heapProperties);
    
    void CreateTexture(ID3D12Device* device, UINT width, UINT height, DXGI_FORMAT format,
                      D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState,
                      const D3D12_HEAP_PROPERTIES* heapProperties);
    
    ID3D12Resource* GetResource() const { return resource.Get(); }
    const D3D12_RESOURCE_DESC& GetDesc() const { return desc; }
    D3D12_RESOURCE_STATES GetCurrentState() const { return currentState; }
    void SetCurrentState(D3D12_RESOURCE_STATES state) { currentState = state; }
    
    void SetResource(ID3D12Resource* res);
    
    // Memory management methods
    void Upload(void* source, size_t size);
    D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress();

private:
    ComPtr<ID3D12Resource> resource;
    D3D12_RESOURCE_DESC desc;
    D3D12_RESOURCE_STATES currentState;
};