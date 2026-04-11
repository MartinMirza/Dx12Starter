#pragma once

#include <d3d12.h>
#include "DX12Device.h"
#include "DX12Resource.h"

class DX12Geometry
{
    friend class DX12CommandList;
public:
    DX12Geometry(DX12Device* device, void* data, UINT vertexSize, UINT numOfVertices);
    ~DX12Geometry();

private:
    UINT numOfVertices;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    DX12Resource* vertexBuffer;
};