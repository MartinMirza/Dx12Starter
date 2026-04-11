#include "DX12PipelineState.h"
#include "Logger.h"
#include "d3dx12.h"

DX12PipelineState::DX12PipelineState(ID3D12Device* device, DX12Shader* vertexShader, DX12Shader* pixelShader)
{
    Logger::GetInstance().Log("Creating DX12PipelineState...\n");
    CreateRootSignature(device);
    CreatePipelineState(device, vertexShader->GetBytecode(), pixelShader->GetBytecode());
    Logger::GetInstance().Log("DX12PipelineState created successfully\n");
}

DX12PipelineState::~DX12PipelineState()
{
    if (pipelineState)
    {
        pipelineState->Release();
        pipelineState = nullptr;
    }
    if (rootSignature)
    {
        rootSignature->Release();
        rootSignature = nullptr;
    }
    Logger::GetInstance().Log("DX12PipelineState destroyed\n");
}

void DX12PipelineState::CreateRootSignature(ID3D12Device* device)
{
    Logger::GetInstance().Log("Creating root signature...\n");
    
    // Create an empty root signature
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    
    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to serialize root signature: 0x%X\n", hr);
        if (error)
        {
            Logger::GetInstance().Log("Error: %s\n", static_cast<char*>(error->GetBufferPointer()));
            error->Release();
        }
        return;
    }
    
    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create root signature: 0x%X\n", hr);
    }
    else
    {
        Logger::GetInstance().Log("Root signature created successfully\n");
    }
    
    if (signature)
    {
        signature->Release();
    }
}

void DX12PipelineState::CreatePipelineState(ID3D12Device* device, D3D12_SHADER_BYTECODE vs, D3D12_SHADER_BYTECODE ps)
{
    Logger::GetInstance().Log("Creating pipeline state...\n");
    
    // Define input layout for POSITION
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
    
    // Set up pipeline state description
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = rootSignature;
    psoDesc.VS = vs;
    psoDesc.PS = ps;
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    
    
    // Create the pipeline state
    HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create pipeline state: 0x%X\n", hr);
    }
    else
    {
        Logger::GetInstance().Log("Pipeline state created successfully\n");
    }
}