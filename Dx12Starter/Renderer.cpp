#include "Renderer.h"
#include "CommandQueue.h"
#include "DX12CommandList.h"
#include "HardwareCapabilities.h"
#include "Logger.h"
#include "RendererSettings.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "dxguid.lib")

Renderer::Renderer(HWND hwnd, const RendererSettings& settings) : commandQueue(nullptr), hwnd(hwnd), settings(settings), currentFrameIndex(0)
{
    CreateDxgiFactory();
    EnumerateAdapters();
    CreateDevice();
    GetCapabilities(); // Check hardware capabilities before creating swap chain
    CreateCommandQueue();
    CreateCommandList();
    CreateSwapChain();
    CreateRtvDescriptorHeap();
    CreateRenderTargetViews();
    LoadShaders();
    DisableDxgiMsgQueueMonitoring();
}

Renderer::~Renderer()
{
    delete commandQueue;
    delete vertexShader;
    delete pixelShader;
}

void Renderer::CreateDxgiFactory()
{
    Logger::GetInstance().Log("Creating DXGI factory...\n");
    HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create DXGI factory: 0x%X\n", hr);
        return;
    }
    Logger::GetInstance().Log("DXGI factory created successfully\n");
}

void Renderer::EnumerateAdapters()
{
    Logger::GetInstance().Log("Enumerating adapters...\n");
    ComPtr<IDXGIAdapter1> currentAdapter;
    UINT index = 0;
    
    while (SUCCEEDED(dxgiFactory->EnumAdapters1(index, &currentAdapter)))
    {
        LogAdapterInfo(currentAdapter);
        EnumerateOutputs(currentAdapter);
        index++;
    }
    
    // Pick the first adapter as our main GPU
    if (index > 0)
    {
        dxgiFactory->EnumAdapters1(0, &dxgiAdapter);
        Logger::GetInstance().Log("Selected adapter 0 as main GPU\n");
    }
    else
    {
        Logger::GetInstance().Log("No adapters found\n");
    }
}

void Renderer::EnumerateOutputs(ComPtr<IDXGIAdapter1> currentAdapter)
{
    ComPtr<IDXGIOutput> output;
    UINT index = 0;
    
    while (SUCCEEDED(currentAdapter->EnumOutputs(index, &output)))
    {
        LogOutputInfo(output);
        index++;
    }
}

void Renderer::LogOutputInfo(ComPtr<IDXGIOutput> output)
{
    DXGI_OUTPUT_DESC outputDesc;
    output->GetDesc(&outputDesc);
    Logger::GetInstance().Log("    Attached Output: %ws\n", outputDesc.DeviceName);
}

void Renderer::LogAdapterInfo(ComPtr<IDXGIAdapter1> adapter)
{
    DXGI_ADAPTER_DESC1 adapterDesc;
    adapter->GetDesc1(&adapterDesc);
    Logger::GetInstance().Log("  %ws (Dedicated VRAM: %zu MB)\n", 
        adapterDesc.Description, 
        adapterDesc.DedicatedVideoMemory / 1024 / 1024);
}

void Renderer::CreateDevice()
{
    Logger::GetInstance().Log("Creating DirectX device...\n");

#if _DEBUG
    EnableDebugLayer();
#endif

    HRESULT hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create DirectX device: 0x%X\n", hr);
        return;
    }
    
    Logger::GetInstance().Log("DirectX device created successfully\n");
}

void Renderer::EnableDebugLayer()
{
    Logger::GetInstance().Log("Enabling DirectX debug layer...\n");

    ComPtr<ID3D12Debug> debugInterface;
    HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to get debug interface: 0x%X\n", hr);
        return;
    }

    debugInterface->EnableDebugLayer();

    ComPtr<ID3D12Debug1> debugInterface1;
    hr = debugInterface.As(&debugInterface1);
    if (SUCCEEDED(hr))
    {
        debugInterface1->SetEnableGPUBasedValidation(true);
    }

    Logger::GetInstance().Log("DirectX debug layer enabled\n");
}

void Renderer::CreateCommandQueue()
{
    Logger::GetInstance().Log("Creating CommandQueue object...\n");
    
    commandQueue = new CommandQueue();
    commandQueue->Initialize(device.Get());
}

void Renderer::CreateCommandList()
{
    Logger::GetInstance().Log("Creating command list...\n");
    commandList = new DX12CommandList(device.Get());
    Logger::GetInstance().Log("Command list created successfully\n");
}

void Renderer::CreateSwapChain()
{
    Logger::GetInstance().Log("Creating swap chain...\n");

    DXGI_SAMPLE_DESC sampleDesc;
    ZeroMemory(&sampleDesc, sizeof(sampleDesc));
    sampleDesc.Count = 1;
    sampleDesc.Quality = 0;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.Width = 0;
    swapChainDesc.Height = 0;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = FRAME_COUNT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.SampleDesc = sampleDesc;
    
    // Set ALLOW_TEARING flag if VSync is disabled and GPU supports it
    if (IsVsyncDisabledAndTearingAllowed())
    {
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        Logger::GetInstance().Log("Creating swap chain with ALLOW_TEARING flag\n");
    }

    ComPtr<IDXGISwapChain1> tempSwapChain;
    HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(
        commandQueue->GetCommandQueue().Get(),
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &tempSwapChain);

    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create swap chain: 0x%X\n", hr);
        return;
    }

    hr = tempSwapChain.As(&dxgiSwapChain);
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to get IDXGISwapChain3 interface: 0x%X\n", hr);
        return;
    }

    Logger::GetInstance().Log("Swap chain created successfully\n");
}

void Renderer::CreateRtvDescriptorHeap()
{
    Logger::GetInstance().Log("Creating RTV descriptor heap...\n");
    rtvDescriptorHeap = new DX12DescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void Renderer::CreateRenderTargetViews()
{
    Logger::GetInstance().Log("Creating render target views...\n");
    
    for (UINT i = 0; i < FRAME_COUNT; i++)
    {
        HRESULT hr = dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&resources[i]));
        if (FAILED(hr))
        {
            Logger::GetInstance().Log("Failed to get back buffer %d: 0x%X\n", i, hr);
            continue;
        }
        
        D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle = rtvDescriptorHeap->AllocateDescriptor();
        renderTargets[i] = new DX12RenderTarget(device.Get(), resources[i].Get(), descriptorHandle, D3D12_RESOURCE_STATE_COMMON);
    }
    
    Logger::GetInstance().Log("Render target views created successfully\n");
}

void Renderer::DisableDxgiMsgQueueMonitoring()
{
    Logger::GetInstance().Log("Disabling DXGI message queue monitoring...\n");

    HRESULT hr = dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to disable DXGI message queue monitoring: 0x%X\n", hr);
        return;
    }

    Logger::GetInstance().Log("DXGI message queue monitoring disabled\n");
}

void Renderer::LoadShaders()
{
    Logger::GetInstance().Log("Loading shaders...\n");
    
    // Load vertex shader
    vertexShader = new DX12Shader("bypass_vs.cso");
    if (!vertexShader)
    {
        Logger::GetInstance().Log("Failed to load vertex shader\n");
        return;
    }
    
    // Load pixel shader
    pixelShader = new DX12Shader("bypass_ps.cso");
    if (!pixelShader)
    {
        Logger::GetInstance().Log("Failed to load pixel shader\n");
        return;
    }
    
    Logger::GetInstance().Log("Shaders loaded successfully\n");
}

void Renderer::Render()
{
    RecordCommandList();
    CloseCommandList();
    ExecuteCommandList();
    PresentFrame();
}

void Renderer::ResetCommandList()
{
    // The DX12CommandList class handles its own reset internally
    commandList->Reset();
}

void Renderer::RecordCommandList()
{
    constexpr float clearColorOne[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    constexpr float clearColorTwo[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    
    commandList->Reset();
    commandList->TransitionTo(renderTargets[currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList->SetRenderTarget(renderTargets[currentFrameIndex]);
    commandList->ClearRenderTarget(renderTargets[currentFrameIndex], currentFrameIndex ? clearColorOne : clearColorTwo);
    commandList->TransitionTo(renderTargets[currentFrameIndex], D3D12_RESOURCE_STATE_PRESENT);
}

void Renderer::CloseCommandList()
{
    commandList->Close();
}

void Renderer::ExecuteCommandList()
{
    if (commandQueue)
    {
        commandQueue->Execute(commandList);
    }
}

void Renderer::GetCapabilities()
{
    Logger::GetInstance().Log("Checking hardware capabilities...\n");
    
    // Check if GPU supports ALLOW_TEARING feature
    BOOL isAllowedTearing = false;
    HRESULT hr = dxgiFactory->CheckFeatureSupport(
        DXGI_FEATURE_PRESENT_ALLOW_TEARING, 
        &isAllowedTearing, 
        sizeof(isAllowedTearing));
    
    if (SUCCEEDED(hr) && isAllowedTearing)
    {
        hardwareCapabilities.SetCapability(HardwareCapabilities::ALLOW_TEARING);
        Logger::GetInstance().Log("GPU supports ALLOW_TEARING feature\n");
    }
    else
    {
        Logger::GetInstance().Log("GPU does not support ALLOW_TEARING feature\n");
    }
}

bool Renderer::IsVsyncDisabledAndTearingAllowed()
{
    return !settings.isVsyncEnabled && hardwareCapabilities.GetCapability(HardwareCapabilities::ALLOW_TEARING);
}

void Renderer::PresentFrame()
{
    if (!dxgiSwapChain)
    {
        Logger::GetInstance().Log("Error: Swap chain not initialized\n");
        return;
    }

    if (IsVsyncDisabledAndTearingAllowed())
    {
        // Present without VSync (allow tearing)
        HRESULT hr = dxgiSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
        if (FAILED(hr))
        {
            Logger::GetInstance().Log("Present failed with tearing: 0x%X\n", hr);
        }
    }
    else
    {
        // Present with VSync (sync interval = 1, but using 4 for better visibility as per tutorial)
        HRESULT hr = dxgiSwapChain->Present(4, 0);
        if (FAILED(hr))
        {
            Logger::GetInstance().Log("Present failed with VSync: 0x%X\n", hr);
        }
    }

    // Update current frame index
    currentFrameIndex = dxgiSwapChain->GetCurrentBackBufferIndex();
}