#include "DX12Device.h"
#include "Logger.h"

DX12Device::DX12Device(ID3D12Device* device)
    : device(device)
{
    Logger::GetInstance().Log("DX12Device created successfully\n");
}

DX12Device::~DX12Device()
{
    Logger::GetInstance().Log("DX12Device destroyed\n");
}

ID3D12Device* DX12Device::GetDevice() const
{
    return device.Get();
}