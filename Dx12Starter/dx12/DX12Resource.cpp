#include "DX12Resource.h"
#include "Logger.h"
#include "d3dx12.h"

DX12Resource::DX12Resource() : currentState(D3D12_RESOURCE_STATE_COMMON)
{
    desc = {};
}

DX12Resource::DX12Resource(ID3D12Device* device, UINT64 bufferSize)
    : currentState(D3D12_RESOURCE_STATE_GENERIC_READ)
{
    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
    
    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&resource)
    );
    
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("ERROR: Failed to create committed resource (HRESULT: 0x%X)\n", hr);
        return;
    }
    
    desc = resource->GetDesc();
    Logger::GetInstance().Log("Committed resource created successfully (Size: %llu bytes)\n", bufferSize);
}

DX12Resource::~DX12Resource()
{
    Logger::GetInstance().Log("DX12Resource destroyed\n");
}

void DX12Resource::CreateBuffer(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_FLAGS flags,
                                D3D12_RESOURCE_STATES initialState, const D3D12_HEAP_PROPERTIES* heapProperties)
{
    Logger::GetInstance().Log("Creating buffer resource (Size: %llu bytes, Flags: 0x%X, State: %d)\n", size, flags, initialState);
    
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = flags;
    
    HRESULT hr = device->CreateCommittedResource(
        heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        initialState,
        nullptr,
        IID_PPV_ARGS(&resource)
    );
    
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("ERROR: Failed to create buffer resource (HRESULT: 0x%X)\n", hr);
        return;
    }
    
    currentState = initialState;
    Logger::GetInstance().Log("Buffer resource created successfully\n");
}

void DX12Resource::CreateTexture(ID3D12Device* device, UINT width, UINT height, DXGI_FORMAT format,
                                 D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState,
                                 const D3D12_HEAP_PROPERTIES* heapProperties)
{
    Logger::GetInstance().Log("Creating texture resource (Width: %d, Height: %d, Format: %d, Flags: 0x%X, State: %d)\n",
                             width, height, format, flags, initialState);
    
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = flags;
    
    HRESULT hr = device->CreateCommittedResource(
        heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        initialState,
        nullptr,
        IID_PPV_ARGS(&resource)
    );
    
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("ERROR: Failed to create texture resource (HRESULT: 0x%X)\n", hr);
        return;
    }
    
    currentState = initialState;
    Logger::GetInstance().Log("Texture resource created successfully\n");
}

void DX12Resource::SetResource(ID3D12Resource* res)
{
    resource = res;
    if (res)
    {
        desc = res->GetDesc();
    }
}

void DX12Resource::Upload(void* source, size_t size)
{
    UINT8* resourceData;
    D3D12_RANGE readRange = {0, 0};
    
    HRESULT hr = resource->Map(0, &readRange, reinterpret_cast<void**>(&resourceData));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("ERROR: Failed to map resource (HRESULT: 0x%X)\n", hr);
        return;
    }
    
    memcpy(resourceData, source, size);
    resource->Unmap(0, nullptr);
    
    Logger::GetInstance().Log("Uploaded %zu bytes to GPU resource\n", size);
}

D3D12_GPU_VIRTUAL_ADDRESS DX12Resource::GetGpuVirtualAddress()
{
    if (!resource)
    {
        Logger::GetInstance().Log("WARNING: Attempting to get GPU virtual address of null resource\n");
        return D3D12_GPU_VIRTUAL_ADDRESS(0);
    }
    
    return resource->GetGPUVirtualAddress();
}