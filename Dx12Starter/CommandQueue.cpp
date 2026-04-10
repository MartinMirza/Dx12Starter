#include "CommandQueue.h"
#include "Logger.h"
#include "DX12CommandList.h"

CommandQueue::CommandQueue() : fenceEvent(nullptr), fenceValue(0)
{
}

CommandQueue::~CommandQueue()
{
    if (fenceEvent != nullptr && fenceEvent != INVALID_HANDLE_VALUE)
    {
        CloseHandle(fenceEvent);
    }
}

void CommandQueue::Initialize(ID3D12Device* device)
{
    Logger::GetInstance().Log("Initializing CommandQueue...\n");
    
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
    Logger::GetInstance().Log("Command queue created\n");

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

    Logger::GetInstance().Log("CommandQueue initialized successfully\n");
}

void CommandQueue::Execute(const ComPtr<ID3D12CommandList>& commandList)
{
    ID3D12CommandList* commandLists[] = { commandList.Get() };
    commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
    Flush();
}

void CommandQueue::Execute(DX12CommandList* commandList)
{
    // DX12CommandList version
    ID3D12CommandList* lists[] = { commandList->GetCommandList() };
    commandQueue->ExecuteCommandLists(_countof(lists), lists);
    Flush(); // Ensure GPU is done before we reset the allocator
}

void CommandQueue::Flush()
{
    Signal();
    WaitForGpu();
}

void CommandQueue::Signal()
{
    HRESULT hr = commandQueue->Signal(fence.Get(), fenceValue);
    if (FAILED(hr))
    {
        Logger::GetInstance().Log("Failed to signal fence: 0x%X\n", hr);
    }
}

void CommandQueue::WaitForGpu()
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