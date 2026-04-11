#include "DX12DescriptorHeap.h"
#include "Logger.h"

DX12DescriptorHeap::DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type)
    : baseIndex(0)
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = HEAP_SIZE;
    heapDesc.Type = type;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create descriptor heap: 0x%X\n", hr);
        return;
    }
    
    descriptorSize = device->GetDescriptorHandleIncrementSize(type);
    Logger::GetInstance().Log("DX12DescriptorHeap created successfully (Type: %d, Size: %d)\n", type, HEAP_SIZE);
}

DX12DescriptorHeap::~DX12DescriptorHeap()
{
    // ComPtr will automatically release the heap
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::AllocateDescriptor()
{
    if (baseIndex >= HEAP_SIZE)
    {
        Logger::GetInstance().Log("Error: Descriptor heap exhausted!\n");
        return {};
    }
    
    D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += baseIndex * descriptorSize;
    baseIndex++;
    
    return handle;
}
