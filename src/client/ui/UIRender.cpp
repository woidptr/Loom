#include "UIRender.hpp"

#include <client/Client.hpp>
#include <imgui_internal.h>

UIRender::UIRender(WindowProcHook* windowProcHook, PresentHook* presentHook, ExecuteCommandListHook* executeCommandListHook, ResizeBuffersHook* resizeBuffersHook) {
    windowProcHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> std::optional<LRESULT> {
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

            switch (msg) {
            case WM_KEYDOWN:
                keyboardCallback((int16_t)wParam, true);

                break;
            case WM_KEYUP:
                keyboardCallback((int16_t)wParam, false);

                break;
            }

            if ((msg >= WM_KEYDOWN && msg <= WM_KEYLAST) && (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)) {
                if (ScreenManager::getCurrentScreen()) {
                    return 1;
                    // cbCtx.cancel();
                }
            }

            return std::nullopt;
            // Logger::info(std::format("Window proc message: {}", msg));
        }
    );

    presentHook->registerCallback(
        [&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
            renderCallback(swapChain, a1, a2);
        }
    );

    executeCommandListHook->registerCallback(
        [&](CallbackContext& cbCtx, ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList) {
            executeCommandListCallback(commandQueue, a1, commandList);
        }
    );

    resizeBuffersHook->registerCallback(
        [&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
            resizeBuffersCallback(swapChain, a1, a2, a3, format, a4);
        }
    );
}

void UIRender::keyboardCallback(int16_t key, bool isDown) {
    if (key == 'L' && !isDown) {
        ScreenManager::setScreen(std::make_unique<CustomizationScreen>());
    }

    if (key == VK_SHIFT && !isDown) {
        ToastManager::addToast("Test", 3.0f);
    }

    if (key == VK_ESCAPE && !isDown) {
        ScreenManager::setScreen(nullptr);
    }
}

void UIRender::initImgui(IDXGISwapChain3* swapChain) {
    if (this->initialized) {
        return;
    }

    if (DX12Context::device) {
        DXGI_SWAP_CHAIN_DESC desc;
        swapChain->GetDesc(&desc);
        Win32Context::window = desc.OutputWindow;

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D11_CREATE_DEVICE_FLAG deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        HRESULT hr = D3D11On12CreateDevice(
            DX12Context::device.get(),
            deviceFlags,
            featureLevels,
            _countof(featureLevels),
            (IUnknown**)&DX12Context::cmdQueue,
            1,
            0,
            DX11Context::device.put(),
            DX11Context::context.put(),
            nullptr
        );

        if (FAILED(hr)) {
            $logInfo("D3D11On12CreateDevice failed");
        }
        /*else
        {
            Logger::info(std::format("D3D11 device: 0x{:X}, context: 0x{:X}", (uintptr_t)this->d3d11Device, (uintptr_t)this->d3d11DeviceContext));
        }*/

        DX11Context::device->QueryInterface(__uuidof(ID3D11On12Device), (void**)&this->d3d11On12Device);

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

        ImGui_ImplWin32_Init(Win32Context::window);
        ImGui_ImplDX11_Init(DX11Context::device.get(), DX11Context::context.get());

        initialized = true;
    }
}

void UIRender::renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
    if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(DX12Context::device.put())))) {
        if (!DX12Context::cmdQueue) {
            return;
        }

        this->initImgui(swapChain);

        ID3D12Resource* backBuffer12 = nullptr;
        swapChain->GetBuffer(swapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&backBuffer12));

        D3D11_RESOURCE_FLAGS flags = { D3D11_BIND_RENDER_TARGET };

        ID3D11Resource* backBuffer11 = nullptr;

        this->d3d11On12Device->CreateWrappedResource(
            backBuffer12,
            &flags,
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            IID_PPV_ARGS(&backBuffer11)
        );

        this->d3d11On12Device->AcquireWrappedResources(&backBuffer11, 1);

        ID3D11RenderTargetView* rtv = nullptr;
        DX11Context::device->CreateRenderTargetView(backBuffer11, nullptr, &rtv);

        DX11Context::context->OMSetRenderTargets(1, &rtv, nullptr);

        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();

        ToastManager::renderToasts();
        ScreenManager::render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ID3D11RenderTargetView* nullRTV[1] = { nullptr };
        DX11Context::context->OMSetRenderTargets(1, nullRTV, nullptr);
        this->d3d11On12Device->ReleaseWrappedResources(&backBuffer11, 1);
        DX11Context::context->Flush();

        backBuffer11->Release();
        backBuffer12->Release();
    }
    else if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(DX11Context::device.put())))) {
        this->initImgui(swapChain);

        ID3D11Resource* backBuffer11 = nullptr;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer11));

        ID3D11RenderTargetView* rtv = nullptr;
        DX11Context::device->CreateRenderTargetView(backBuffer11, nullptr, &rtv);

        DX11Context::context->OMSetRenderTargets(1, &rtv, nullptr);

        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();

        ToastManager::renderToasts();
        ScreenManager::render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ID3D11RenderTargetView* nullRTV[1] = { nullptr };
        DX11Context::context->OMSetRenderTargets(1, nullRTV, nullptr);
        DX11Context::context->Flush();

        backBuffer11->Release();
    }
}

void UIRender::executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList) {
    if (!DX12Context::cmdQueue) {
        DX12Context::cmdQueue = commandQueue;
    }

    if (!DX12Context::cmdList) {
        DX12Context::cmdList = *commandList;
    }
}

void UIRender::resizeBuffersCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
    if (!this->initialized) {
        return;
    }
}