#pragma once

#include <d3d12.h>
#include "DX12Resource.h"

class DX12RenderTarget
{
    friend class DX12CommandList;
public:
    DX12RenderTarget(ID3D12Device* device, DX12Resource* resource, 
                    D3D12_CPU_DESCRIPTOR_HANDLE descriptor, D3D12_RESOURCE_STATES state);
    ~DX12RenderTarget();

    ID3D12Resource* GetResource() const;
    DX12Resource* GetDX12Resource() const { return resource; }
    D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor() const;
    D3D12_RESOURCE_STATES GetState() const;

private:
    DX12Resource* resource;
    D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
};
