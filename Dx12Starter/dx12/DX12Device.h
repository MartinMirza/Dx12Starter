#pragma once

#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class DX12Device
{
public:
    DX12Device(ID3D12Device* device);
    ~DX12Device();
    
    ID3D12Device* GetDevice() const;
    
private:
    ComPtr<ID3D12Device> device;
};