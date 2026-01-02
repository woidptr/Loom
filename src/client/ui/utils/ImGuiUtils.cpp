#pragma once
#include "ImGuiUtils.hpp"

void ImGuiDX12::Init(IDXGISwapChain3* swapChain) {

}

void ImGuiDX11::Init(IDXGISwapChain3* swapChain) {
    if (initialized) {
        return;
    }

    swapChain->GetDevice(IID_PPV_ARGS(ImGuiDX11::device.put()));
    ImGuiDX11::device->GetImmediateContext(ImGuiDX11::context.put());

    CreateRenderTarget(swapChain);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    const Asset arimoFont = $get_asset(fonts_Arimo_Medium_ttf);

    io.Fonts->AddFontFromMemoryTTF(
        (void*)arimoFont.begin(),
        arimoFont.size(),
        16.0f
    );

    DXGI_SWAP_CHAIN_DESC desc;
    swapChain->GetDesc(&desc);

    ImGui_ImplWin32_Init(desc.OutputWindow);
    ImGui_ImplDX11_Init(ImGuiDX11::device.get(), ImGuiDX11::context.get());

    initialized = true;
}

void ImGuiDX11::PreRender(IDXGISwapChain3* swapChain) {
    if (!rtv) {
        CreateRenderTarget(swapChain);
    }

    ID3D11RenderTargetView* rtvs[] = { rtv.get() };
    ImGuiDX11::context->OMSetRenderTargets(1, rtvs, nullptr);

    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
}

void ImGuiDX11::Render(IDXGISwapChain3* swapChain) {
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ID3D11RenderTargetView* nullRTV[1] = { nullptr };
    ImGuiDX11::context->OMSetRenderTargets(1, nullRTV, nullptr);
    ImGuiDX11::context->Flush();
}

void ImGuiDX11::PreResize(IDXGISwapChain3* swapChain) {
    ImGui_ImplDX11_InvalidateDeviceObjects();

    context->OMSetRenderTargets(0, nullptr, nullptr);

    rtv = nullptr;
    backBuffer = nullptr;
}

void ImGuiDX11::PostResize(IDXGISwapChain3* swapChain) {
    CreateRenderTarget(swapChain);
}

void ImGuiDX11::CreateRenderTarget(IDXGISwapChain3* swapChain) {
    backBuffer = nullptr;
    rtv = nullptr;

    swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.put()));
    device->CreateRenderTargetView(backBuffer.get(), nullptr, rtv.put());
}