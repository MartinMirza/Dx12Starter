#include "DX12CommandQueue.h"
#include "Logger.h"
#include "DX12CommandList.h"

DX12CommandQueue::DX12CommandQueue() : fenceEvent(nullptr), fenceValue(0)
{
    Logger::GetInstance().Log("DX12CommandQueue created\n");
}

DX12CommandQueue::~DX12CommandQueue()
{
    if (fenceEvent != nullptr && fenceEvent != INVALID_HANDLE_VALUE)
    {
        CloseHandle(fenceEvent);
    }
    Logger::GetInstance().Log("DX12CommandQueue destroyed\n");
}

void DX12CommandQueue::Initialize(ID3D12Device* device)
{
    Logger::GetInstance().Log("Initializing DX12CommandQueue...\n");
    
    // Create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create command queue: 0x%X\n", hr);
        return;
    }
    Logger::GetInstance().Log("DX12CommandQueue created\n");

    // Create fence
    hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to create fence: 0x%X\n", hr);
        return;
    }
    Logger::GetInstance().Log("Fence created\n");

    // Create event object
    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr)
    {
        Logger::GetInstance().Log("Failed to create fence event\n");
        return;
    }
    Logger::GetInstance().Log("Fence event created\n");

    Logger::GetInstance().Log("DX12CommandQueue initialized successfully\n");
}

void DX12CommandQueue::Execute(DX12CommandList* commandList)
{
    // DX12CommandList version
    ID3D12CommandList* lists[] = { commandList->GetCommandList() };
    commandQueue->ExecuteCommandLists(_countof(lists), lists);
    Flush(); // Ensure GPU is done before we reset the allocator
}

void DX12CommandQueue::Flush()
{
    Signal();
    WaitForGpu();
}

void DX12CommandQueue::Signal()
{
    HRESULT hr = commandQueue->Signal(fence.Get(), fenceValue);
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to signal fence: 0x%X\n", hr);
    }
}

void DX12CommandQueue::WaitForGpu()
{
    if (fence->GetCompletedValue() < fenceValue)
    {
        HRESULT hr = fence->SetEventOnCompletion(fenceValue, fenceEvent);
        if (FAILED(hr))
        {
            Logger::GetInstance().Log("Failed to set fence event: 0x%X\n", hr);
            return;
        }

        WaitForSingleObject(fenceEvent, INFINITE);
    }
    fenceValue++;
}