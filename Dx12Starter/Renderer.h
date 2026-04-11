#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

#include "DX12DescriptorHeap.h"
#include "DX12RenderTarget.h"
#include "DX12Shader.h"
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

    ComPtr<IDXGIFactory5> dxgiFactory;
    ComPtr<IDXGIAdapter1> dxgiAdapter;
    ComPtr<ID3D12Device> device;
    CommandQueue* commandQueue;
    DX12CommandList* commandList;
    ComPtr<IDXGISwapChain3> dxgiSwapChain;
    DX12DescriptorHeap* rtvDescriptorHeap = nullptr;
    DX12RenderTarget* renderTargets[FRAME_COUNT] = { nullptr };
    ComPtr<ID3D12Resource> resources[FRAME_COUNT];
    DX12Shader* vertexShader = nullptr;
    DX12Shader* pixelShader = nullptr;
    HWND hwnd;
    HardwareCapabilities hardwareCapabilities;
    RendererSettings settings;
    UINT currentFrameIndex = 0;
};