#pragma once
#include <core/Asset.hpp>
#include <winrt/base.h>

#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

$load_asset(fonts_Arimo_Medium_ttf);

class IImguiRenderer {
public:
    virtual ~IImguiRenderer() = default;

    virtual void Init(IDXGISwapChain3* swapChain) = 0;
    virtual void PreRender(IDXGISwapChain3* swapChain) = 0;
    virtual void Render(IDXGISwapChain3* swapChain) = 0;
    virtual void PreResize(IDXGISwapChain3* swapChain) = 0;
    virtual void PostResize(IDXGISwapChain3* swapChain) = 0;
};

class ImGuiDX12 : public IImguiRenderer {
private:
    bool initialized = false;
    winrt::com_ptr<ID3D12Device> device = nullptr;
    winrt::com_ptr<ID3D12CommandQueue> cmdQueue = nullptr;
    winrt::com_ptr<ID3D12DescriptorHeap> rtvHeap = nullptr;
    UINT backBufferCount = 0;

    struct FrameContext {
        winrt::com_ptr<ID3D12CommandAllocator> allocator;
        winrt::com_ptr<ID3D12GraphicsCommandList> cmdList;
        winrt::com_ptr<ID3D12DescriptorHeap> rtvHeap;
    };

    std::vector<FrameContext> frameCommands;
public:
    ImGuiDX12() : IImguiRenderer() {}

    void Init(IDXGISwapChain3* swapChain) override;
    void PreRender(IDXGISwapChain3* swapChain) override {}
    void Render(IDXGISwapChain3* swapChain) override {}
    void PreResize(IDXGISwapChain3* swapChain) override {}
    void PostResize(IDXGISwapChain3* swapChain) override {}
};

class ImGuiDX11 : public IImguiRenderer {
private:
    bool initialized = false;
    winrt::com_ptr<ID3D11Device> device = nullptr;
    winrt::com_ptr<ID3D11DeviceContext> context = nullptr;
    winrt::com_ptr<ID3D11Resource> backBuffer = nullptr;
    winrt::com_ptr<ID3D11RenderTargetView> rtv = nullptr;
public:
    ImGuiDX11() : IImguiRenderer() {}

    void Init(IDXGISwapChain3* swapChain) override;
    void PreRender(IDXGISwapChain3* swapChain) override;
    void Render(IDXGISwapChain3* swapChain) override;
    void PreResize(IDXGISwapChain3* swapChain) override;
    void PostResize(IDXGISwapChain3* swapChain) override;
private:
    void CreateRenderTarget(IDXGISwapChain3* swapChain);
};