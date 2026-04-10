#include "DX12RenderTarget.h"
#include "Logger.h"

DX12RenderTarget::DX12RenderTarget(ID3D12Device* device, ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, D3D12_RESOURCE_STATES state)
    : resource(resource), descriptorHandle(descriptor), resourceState(state)
{
    device->CreateRenderTargetView(resource, nullptr, descriptorHandle);
    Logger::GetInstance().Log("DX12RenderTarget created successfully\n");
}

DX12RenderTarget::~DX12RenderTarget()
{
    // Resource is managed externally, so we don't delete it here
}
