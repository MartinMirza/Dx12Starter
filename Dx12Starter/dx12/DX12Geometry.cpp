#include "DX12Geometry.h"
#include "Logger.h"

DX12Geometry::DX12Geometry(DX12Device* device, void* data, UINT vertexSize, UINT numOfVertices)
    : numOfVertices(numOfVertices), vertexBuffer(nullptr)
{
    if (!device || !data || vertexSize == 0 || numOfVertices == 0)
    {
        Logger::GetInstance().Log("ERROR: Invalid parameters for DX12Geometry\n");
        return;
    }
    
    // Calculate total buffer size
    UINT64 bufferSize = static_cast<UINT64>(vertexSize) * numOfVertices;
    
    // Create vertex buffer resource
    vertexBuffer = new DX12Resource(device->GetDevice(), bufferSize);
    
    if (!vertexBuffer)
    {
        Logger::GetInstance().Log("ERROR: Failed to create vertex buffer resource\n");
        return;
    }
    
    // Upload vertex data
    vertexBuffer->Upload(data, bufferSize);
    
    // Create vertex buffer view
    vertexBufferView.BufferLocation = vertexBuffer->GetGpuVirtualAddress();
    vertexBufferView.SizeInBytes = bufferSize;
    vertexBufferView.StrideInBytes = vertexSize;
    
    Logger::GetInstance().Log("DX12Geometry created successfully (Vertices: %d, Size: %llu bytes)\n", numOfVertices, bufferSize);
}

DX12Geometry::~DX12Geometry()
{
    if (vertexBuffer)
    {
        delete vertexBuffer;
        vertexBuffer = nullptr;
    }
    Logger::GetInstance().Log("DX12Geometry destroyed\n");
}