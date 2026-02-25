#pragma once
#include <winrt/base.h>

#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

class IImguiRenderer {
public:
    virtual ~IImguiRenderer() = default;

    virtual bool Init(IDXGISwapChain3* swapChain, void* commandQueue = nullptr) = 0;

    virtual void NewFrame(IDXGISwapChain3* swapChain) = 0;
    virtual void RenderDrawData(IDXGISwapChain3* swapChain) = 0;

    virtual void OnResizePre(IDXGISwapChain3* swapChain) = 0;
    virtual void OnResizePost(IDXGISwapChain3* swapChain) = 0;

    virtual bool IsReady() const { return true; }
};

struct FrameContext {
    winrt::com_ptr<ID3D12CommandAllocator> commandAllocator = nullptr;
    winrt::com_ptr<ID3D12Resource> mainRenderTargetResource = nullptr;
    D3D12_CPU_DESCRIPTOR_HANDLE mainRenderTargetDescriptor;
};

class ImGuiDX12 : public IImguiRenderer {
private:
    winrt::com_ptr<ID3D12Device> device = nullptr;
    winrt::com_ptr<ID3D12DescriptorHeap> rtvDescHeap = nullptr;
    winrt::com_ptr<ID3D12DescriptorHeap> srvDescHeap = nullptr;
    winrt::com_ptr<ID3D12CommandQueue> cmdQueue = nullptr;
    winrt::com_ptr<ID3D12GraphicsCommandList> cmdList = nullptr;
    winrt::com_ptr<ID3D12Fence> fence = nullptr;

    UINT64 fenceValue = 0;
    HANDLE fenceEvent = nullptr;
    std::vector<UINT64> frameFenceValues;

    std::vector<FrameContext> frameContext;

    UINT bufferCount = 0;
public:
    ImGuiDX12() : IImguiRenderer() {}

    bool Init(IDXGISwapChain3* swapChain, void* commandQueue = nullptr) override;

    void NewFrame(IDXGISwapChain3* swapChain) override;
    void RenderDrawData(IDXGISwapChain3* swapChain) override;

    void OnResizePre(IDXGISwapChain3* swapChain) override;
    void OnResizePost(IDXGISwapChain3* swapChain) override;

    bool IsReady() const override;
private:
    void SetupRenderTargets(IDXGISwapChain3* swapChain);
    void CleanupRenderTargets();
};

class ImGuiDX11 : public IImguiRenderer {
private:
    winrt::com_ptr<ID3D11Device> device = nullptr;
    winrt::com_ptr<ID3D11DeviceContext> context = nullptr;
    winrt::com_ptr<ID3D11Resource> backBuffer = nullptr;
    winrt::com_ptr<ID3D11RenderTargetView> rtv = nullptr;
public:
    ImGuiDX11() : IImguiRenderer() {}

    bool Init(IDXGISwapChain3* swapChain, void* commandQueue = nullptr) override;

    void NewFrame(IDXGISwapChain3* swapChain) override;
    void RenderDrawData(IDXGISwapChain3* swapChain) override;

    void OnResizePre(IDXGISwapChain3* swapChain) override;
    void OnResizePost(IDXGISwapChain3* swapChain) override;
private:
    void SetupRenderTargets(IDXGISwapChain3* swapChain);
    void CleanupRenderTargets();
};