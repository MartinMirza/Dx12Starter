#pragma once

#include <d3d12.h>

class DX12RenderTarget
{
    friend class DX12CommandList;
public:
    DX12RenderTarget(ID3D12Device* device, ID3D12Resource* resource, 
                    D3D12_CPU_DESCRIPTOR_HANDLE descriptor, D3D12_RESOURCE_STATES state);
    ~DX12RenderTarget();

private:
    ID3D12Resource* resource;
    D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
    D3D12_RESOURCE_STATES resourceState;
};
