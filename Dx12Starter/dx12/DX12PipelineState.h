#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>
#include "DX12Shader.h"
#include "Logger.h"

#pragma comment(lib, "D3DCompiler.lib")

class DX12PipelineState
{
    friend class DX12CommandList;
public:
    DX12PipelineState(ID3D12Device* device, DX12Shader* vertexShader, DX12Shader* pixelShader);
    ~DX12PipelineState();

private:
    void CreateShaderSignature(ID3D12Device* device);
    void CreatePipelineState(ID3D12Device* device, D3D12_SHADER_BYTECODE vs, D3D12_SHADER_BYTECODE ps);
    
    ID3D12RootSignature* rootSignature;
    ID3D12PipelineState* pipelineState;
};