#pragma once

#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class DX12DescriptorHeap
{
public:
    DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type);
    ~DX12DescriptorHeap();
    
    D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor();
    
private:
    ComPtr<ID3D12DescriptorHeap> heap;
    UINT descriptorSize;
    UINT baseIndex;
    static constexpr UINT HEAP_SIZE = 1000; // Fixed size as per tutorial
};
