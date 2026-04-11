#include "DX12RenderTarget.h"
#include "Logger.h"

DX12RenderTarget::DX12RenderTarget(ID3D12Device* device, DX12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, D3D12_RESOURCE_STATES state)
    : resource(resource), descriptorHandle(descriptor)
{
    device->CreateRenderTargetView(resource->GetResource(), nullptr, descriptorHandle);
    Logger::GetInstance().Log("DX12RenderTarget created successfully\n");
}

DX12RenderTarget::~DX12RenderTarget()
{
    // Resource is managed by DX12Resource class
    delete resource;
}

ID3D12Resource* DX12RenderTarget::GetResource() const
{
    return resource->GetResource();
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12RenderTarget::GetDescriptor() const
{
    return descriptorHandle;
}

D3D12_RESOURCE_STATES DX12RenderTarget::GetState() const
{
    return resource->GetCurrentState();
}