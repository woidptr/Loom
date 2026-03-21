#include "InputHooks.hpp"

#include <dxgi.h>
#include <GameInput.h>
#include <hooks/HookManager.hpp>
#include <hooks/InlineHook.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/input/MouseEvent.hpp>
#include <winrt/base.h>
#include <kiero.hpp>
#include <events/input/WindowProcessEvent.hpp>

namespace InputHooks {
    LONG_PTR _wnd_proc_hook;
    LRESULT _wnd_proc_detour(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        {
            WindowProcessEvent event{
                .hWnd = hwnd,
                .msg = msg,
                .wParam = wParam,
                .lParam = lParam,
            };

            EventHandler::emit(event);
        }

        // $log_info("Testing window proc message");
        // $log_info("Window process message: {}", msg);

        auto get_exact_key = [](WPARAM wParam, LPARAM lParam) -> uint32_t {
            switch (const uint32_t key = static_cast<uint32_t>(wParam)) {
                case VK_SHIFT: {
                    const uint32_t scan_code = (lParam & 0x00FF0000) >> 16;
                    return MapVirtualKey(scan_code, MAPVK_VSC_TO_VK_EX);
                }
                case VK_CONTROL: {
                    const bool isExtended = (lParam & 0x01000000) != 0;
                    return isExtended ? VK_RCONTROL : VK_LCONTROL;
                }
                case VK_MENU: {
                    const bool isExtended = (lParam & 0x01000000) != 0;
                    return isExtended ? VK_RMENU : VK_LMENU;
                }
                default:
                    return key;
            }
        };

        if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP) {
            const uint32_t key = get_exact_key(wParam, lParam);
            const bool is_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) ? true : false;

            KeyboardEvent event {
                .key = key,
                .isDown = is_down,
            };

            EventHandler::emit(event);

            if (event.isCancelled()) {
                return 0;
            }
        }

        return CallWindowProc(reinterpret_cast<WNDPROC>(_wnd_proc_hook), hwnd, msg, wParam, lParam);
    }

    InlineHook<decltype(&GameInput::v2::IGameInputReading::GetMouseState)> _reading_hook;
    bool _reading_detour(GameInput::v2::IGameInputReading* self, GameInput::v2::GameInputMouseState* mouseState) {
        MouseEvent event {
            .actionButtonId = mouseState->buttons,
        };

        EventHandler::emit(event);

        if (event.isCancelled()) {
            return false;
        }

        return _reading_hook.call(self, mouseState);
    }

    void init() {
        [&]() {
            winrt::com_ptr<GameInput::v2::IGameInput> gameInput;
            if (FAILED(GameInputCreate(gameInput.put()))) {
                $log_error("Failed to create dummy IGameInput");
                return;
            }

            winrt::com_ptr<GameInput::v2::IGameInputReading> gameInputReading;
            if (FAILED(gameInput->GetCurrentReading(GameInput::v2::GameInputKind::GameInputKindMouse, nullptr, gameInputReading.put()))) {
                $log_error("Failed to create dummy IGameInputReading");
                return;
            }

            // constexpr auto vtable_index = kiero::detail::magic_vft::vtable_index<&GameInput::v2::IGameInputReading::GetMouseState>();
            constexpr size_t vtable_index = 14;
            void** vtable = *reinterpret_cast<void***>(gameInputReading.get());
            void* address = vtable[vtable_index];

            HookManager::createInlineHook(_reading_hook.getHook(), address, &_reading_detour);
        }();

        _wnd_proc_hook = SetWindowLongPtrA(FindWindowA("Bedrock", "Minecraft"), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&_wnd_proc_detour));
    }
}