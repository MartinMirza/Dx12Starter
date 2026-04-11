#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

#include "dx12\DX12DescriptorHeap.h"
#include "dx12\DX12RenderTarget.h"
#include "dx12\DX12Resource.h"
#include "dx12\DX12Shader.h"
#include "dx12\DX12PipelineState.h"
#include "HardwareCapabilities.h"
#include "RendererSettings.h"

class CommandQueue;

class Renderer
{
public:
    Renderer(HWND hwnd, const RendererSettings& settings);
    ~Renderer();
    void Render();
    
private:
    static constexpr UINT FRAME_COUNT = 2;
    
    void ResetCommandList();
    void RecordCommandList();
    void CloseCommandList();
    void ExecuteCommandList();
    void CreateDxgiFactory();
    void EnumerateAdapters();
    void PresentFrame();
    void EnumerateOutputs(ComPtr<IDXGIAdapter1> currentAdapter);
    void LogOutputInfo(ComPtr<IDXGIOutput> output);
    void LogAdapterInfo(ComPtr<IDXGIAdapter1> adapter);
    void CreateDevice();
    void EnableDebugLayer();
    void CreateCommandQueue();
    void CreateCommandList();
    void CreateSwapChain();
    void CreateRTVDescriptorHeap();
    void CreateRenderTargetViews();
    void DisableDxgiMsgQueueMonitoring();
    void GetCapabilities();
    bool IsVsyncDisabledAndTearingAllowed();
    void LoadShaders();
    void CreatePipelineState();
    void TestMemoryAllocation();

    ComPtr<IDXGIFactory5> dxgiFactory;
    ComPtr<IDXGIAdapter1> dxgiAdapter;
    ComPtr<ID3D12Device> device;
    CommandQueue* commandQueue;
    DX12CommandList* commandList;
    ComPtr<IDXGISwapChain3> dxgiSwapChain;
    DX12DescriptorHeap* rtvDescriptorHeap = nullptr;
    DX12RenderTarget* renderTargets[FRAME_COUNT] = { nullptr };
    DX12Shader* vertexShader = nullptr;
    DX12Shader* pixelShader = nullptr;
    DX12PipelineState* pipelineState = nullptr;
    DX12Resource* testBuffer = nullptr; // For memory allocation testing
    HWND hwnd;
    HardwareCapabilities hardwareCapabilities;
    RendererSettings settings;
    UINT currentFrameIndex = 0;
};