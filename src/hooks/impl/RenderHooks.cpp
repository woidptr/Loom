#include "RenderHooks.hpp"
#include <hooks/HookManager.hpp>
#include <hooks/InlineHook.hpp>
#include <events/render/ExecuteCommandListsEvent.hpp>
#include <events/render/PresentEvent.hpp>
#include <events/render/ResizeBuffersEvent.hpp>
#include <events/render/SetupAndRenderEvent.hpp>
#include <kiero.hpp>

namespace RenderHooks {
    InlineHook<void(ScreenView*, MinecraftUIRenderContext*)> _ScreenView_setupAndRender_hook;
    void _ScreenView_setupAndRender_detour(ScreenView* screenView, MinecraftUIRenderContext* renderCtx) {
        GameContext::registerClientInstance(renderCtx->mClient);

        SetupAndRenderEvent event{
            .screenView = screenView,
            .renderCtx = renderCtx,
        };

        EventHandler::emit(event);

        if (event.isCancelled()) {
            return;
        }
        else {
            return _ScreenView_setupAndRender_hook.call(screenView, renderCtx);
        }
    }

    InlineHook<HRESULT(IDXGISwapChain3*, UINT, UINT)> _present_hook;
    HRESULT _present_detour(IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
        PresentEvent event{
            .swapChain = swapChain,
        };

        EventHandler::emit(event);

        return _present_hook.call(swapChain, a1, a2);
    }

    InlineHook<decltype(&IDXGISwapChain3::ResizeBuffers)> _resize_buffers_hook;
    HRESULT _resize_buffers_detour(IDXGISwapChain3* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
        ResizeBuffersEvent beforeEvent{
            .before = true,
            .swapChain = swapChain,
        };
        EventHandler::emit(beforeEvent);

        HRESULT result = _resize_buffers_hook.call(swapChain, bufferCount, width, height, newFormat, swapChainFlags);

        ResizeBuffersEvent afterEvent{
            .before = false,
            .swapChain = swapChain,
        };
        EventHandler::emit(afterEvent);

        return result;
    }

    InlineHook<decltype(&ID3D12CommandQueue::ExecuteCommandLists)> _execute_command_lists_hook;
    void _execute_command_lists_detour(ID3D12CommandQueue* cmdQueue, UINT numCmdLists, ID3D12CommandList** const ppCmdLists) {
        ExecuteCommandListsEvent event{
            .cmdQueue = cmdQueue,
        };

        EventHandler::emit(event);

        return _execute_command_lists_hook.call(cmdQueue, numCmdLists, ppCmdLists);
    }

    void init() {
        HookManager::createInlineHook(_ScreenView_setupAndRender_hook.getHook(), (void*)$get_address("ScreenView::setupAndRender"), &_ScreenView_setupAndRender_detour);

        // DirectX hooks
        HookManager::createInlineHook(_present_hook.getHook(), (void*)kiero::getMethod<&IDXGISwapChain3::Present>(), &_present_detour);
        HookManager::createInlineHook(_resize_buffers_hook.getHook(), (void*)kiero::getMethod<&IDXGISwapChain3::ResizeBuffers>(), &_resize_buffers_detour);

        if (kiero::getRenderType() == kiero::RenderType::D3D12) {
            HookManager::createInlineHook(_execute_command_lists_hook.getHook(), (void*)kiero::getMethod<&ID3D12CommandQueue::ExecuteCommandLists>(), &_execute_command_lists_detour);
        }
    }
}