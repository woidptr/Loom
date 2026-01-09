#pragma once
#include "ImGuiUtils.hpp"
#include <core/Logger.hpp>

bool ImGuiDX12::IsReady() const {
    return cmdQueue != nullptr;
}

bool ImGuiDX12::Init(IDXGISwapChain3* swapChain, void* commandQueue) {
    if (!commandQueue) return false;

    auto rawQueue = static_cast<ID3D12CommandQueue*>(commandQueue);
    cmdQueue.copy_from(rawQueue);

    if (!IsReady()) {
        return false;
    }

    if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(device.put())))) {
        DXGI_SWAP_CHAIN_DESC desc;
        swapChain->GetDesc(&desc);

        D3D12_DESCRIPTOR_HEAP_DESC srvDesc = {
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
            .NumDescriptors = 1,
            .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
            .NodeMask = 1,
        };

        if (FAILED(device->CreateDescriptorHeap(&srvDesc, IID_PPV_ARGS(srvDescHeap.put())))) {
            return false;
        }

        SetupRenderTargets(swapChain);

        if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, frameContext[0].commandAllocator.get(), nullptr, IID_PPV_ARGS(cmdList.put())))) {
            return false;
        }

        cmdList->Close();

        if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.put())))) {
            return false;
        }

        fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (fenceEvent == nullptr) return false;

        frameFenceValues.resize(bufferCount, 0);

        ImGui::CreateContext();

        ImGui_ImplWin32_Init(desc.OutputWindow);

        ImGui_ImplDX12_InitInfo initInfo;
        initInfo.Device = device.get();
        initInfo.CommandQueue = cmdQueue.get();
        initInfo.NumFramesInFlight = bufferCount;
        initInfo.RTVFormat = desc.BufferDesc.Format;
        initInfo.DSVFormat = DXGI_FORMAT_UNKNOWN;

        initInfo.SrvDescriptorHeap = srvDescHeap.get();
        initInfo.LegacySingleSrvCpuDescriptor = srvDescHeap->GetCPUDescriptorHandleForHeapStart();
        initInfo.LegacySingleSrvGpuDescriptor = srvDescHeap->GetGPUDescriptorHandleForHeapStart();

        ImGui_ImplDX12_Init(&initInfo);

        return true;
    }

    return false;
}

void ImGuiDX12::CleanupRenderTargets() {
    fenceValue++;
    cmdQueue->Signal(fence.get(), fenceValue);
    fence->SetEventOnCompletion(fenceValue, fenceEvent);
    WaitForSingleObject(fenceEvent, INFINITE);

    for (FrameContext& frame : frameContext) {
        if (frame.mainRenderTargetResource) frame.mainRenderTargetResource = nullptr;
        if (frame.commandAllocator) frame.commandAllocator = nullptr;
    }

    frameContext.clear();

    rtvDescHeap = nullptr;
}

void ImGuiDX12::NewFrame(IDXGISwapChain3 *swapChain) {
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX12_NewFrame();
}

void ImGuiDX12::RenderDrawData(IDXGISwapChain3 *swapChain) {
    UINT backBufferIdx = swapChain->GetCurrentBackBufferIndex();

    if (fence->GetCompletedValue() < frameFenceValues[backBufferIdx]) {
        if (FAILED(fence->SetEventOnCompletion(frameFenceValues[backBufferIdx], fenceEvent))) {
            return;
        }
        WaitForSingleObject(fenceEvent, INFINITE);
    }

    FrameContext& currentFrame = frameContext[backBufferIdx];

    currentFrame.commandAllocator->Reset();
    cmdList->Reset(currentFrame.commandAllocator.get(), nullptr);

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = currentFrame.mainRenderTargetResource.get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    cmdList->ResourceBarrier(1, &barrier);

    cmdList->OMSetRenderTargets(1, &currentFrame.mainRenderTargetDescriptor, FALSE, nullptr);
    ID3D12DescriptorHeap* heaps[] = { srvDescHeap.get() };
    cmdList->SetDescriptorHeaps(1, heaps);

    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.get());

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    cmdList->ResourceBarrier(1, &barrier);

    cmdList->Close();
    ID3D12CommandList* lists[] = { cmdList.get() };
    cmdQueue->ExecuteCommandLists(1, lists);

    fenceValue++;
    if (FAILED(cmdQueue->Signal(fence.get(), fenceValue))) {
        return;
    }

    frameFenceValues[backBufferIdx] = fenceValue;
}

void ImGuiDX12::OnResizePre(IDXGISwapChain3* swapChain) {
    ImGui_ImplDX12_InvalidateDeviceObjects();
    CleanupRenderTargets();
    ImGui_ImplDX12_Shutdown();
}

void ImGuiDX12::OnResizePost(IDXGISwapChain3 *swapChain) {
    Init(swapChain, cmdQueue.get());
    // SetupRenderTargets(swapChain);
    // ImGui_ImplDX12_CreateDeviceObjects();
}

void ImGuiDX12::SetupRenderTargets(IDXGISwapChain3* swapChain) {
    DXGI_SWAP_CHAIN_DESC desc = {};
    swapChain->GetDesc(&desc);

    bufferCount = desc.BufferCount;
    frameContext.clear();
    frameFenceValues.clear();
    frameContext.resize(bufferCount);
    frameFenceValues.resize(bufferCount, 0);

    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {
        .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
        .NumDescriptors = desc.BufferCount,
        .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        .NodeMask = 1,
    };

    device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(rtvDescHeap.put()));

    UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < bufferCount; i++) {
        frameContext[i].mainRenderTargetDescriptor = rtvHandle;

        swapChain->GetBuffer(i, IID_PPV_ARGS(&frameContext[i].mainRenderTargetResource));

        device->CreateRenderTargetView(frameContext[i].mainRenderTargetResource.get(), nullptr, rtvHandle);

        device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameContext[i].commandAllocator));

        rtvHandle.ptr += rtvDescriptorSize;
    }
}

bool ImGuiDX11::Init(IDXGISwapChain3* swapChain, void* commandQueue) {
    swapChain->GetDevice(IID_PPV_ARGS(ImGuiDX11::device.put()));
    ImGuiDX11::device->GetImmediateContext(ImGuiDX11::context.put());
    
    SetupRenderTargets(swapChain);
    
    ImGui::CreateContext();
    
    DXGI_SWAP_CHAIN_DESC desc;
    swapChain->GetDesc(&desc);
    
    ImGui_ImplWin32_Init(desc.OutputWindow);
    ImGui_ImplDX11_Init(ImGuiDX11::device.get(), ImGuiDX11::context.get());

    return true;
}

void ImGuiDX11::NewFrame(IDXGISwapChain3* swapChain) {
    ID3D11RenderTargetView* rtvs[] = { rtv.get() };
    ImGuiDX11::context->OMSetRenderTargets(1, rtvs, nullptr);
    
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
}

void ImGuiDX11::RenderDrawData(IDXGISwapChain3* swapChain) {
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    ID3D11RenderTargetView* nullRTV[1] = { nullptr };
    ImGuiDX11::context->OMSetRenderTargets(1, nullRTV, nullptr);
    ImGuiDX11::context->Flush();
}

void ImGuiDX11::OnResizePre(IDXGISwapChain3* swapChain) {
    ImGui_ImplDX11_InvalidateDeviceObjects();
    CleanupRenderTargets();
}

void ImGuiDX11::OnResizePost(IDXGISwapChain3* swapChain) {
    SetupRenderTargets(swapChain);
    ImGui_ImplDX11_CreateDeviceObjects();
}

void ImGuiDX11::SetupRenderTargets(IDXGISwapChain3* swapChain) {
    swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.put()));
    device->CreateRenderTargetView(backBuffer.get(), nullptr, rtv.put());
}

void ImGuiDX11::CleanupRenderTargets() {
    context->OMSetRenderTargets(0, nullptr, nullptr);

    rtv = nullptr;
    backBuffer = nullptr;
}