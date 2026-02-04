#include "InputHooks.hpp"
#include <hooks/HookManager.hpp>
#include <hooks/InlineHook.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/input/MouseEvent.hpp>

namespace InputHooks {
    InlineHook<void(int16_t, bool)> _keyboard_feed_hook;
    void _keyboard_feed_detour(int16_t key, bool isDown) {
        KeyboardEvent event{
            .key = key,
            .isDown = isDown,
        };

        EventHandler::emit(event);

        $log_debug("Key is pressed");

        if (event.isCancelled()) {
            return;
        } else {
            return _keyboard_feed_hook.call(key, isDown);
        }
    }

    InlineHook<void(void*, char, char, short, short, short, short, bool)> _mouse_device_feed_hook;
    void _mouse_device_feed_detour(void* mouseDevice, char actionButtonId, char buttonData, short x, short y, short dx, short dy, bool forceMotionlessPointer) {
        MouseEvent event{
            .actionButtonId = actionButtonId,
            .buttonData = buttonData,
            .x = x,
            .y = y,
            .dx = dx,
            .dy = dy,
            .forceMotionlessPointer = forceMotionlessPointer,
        };

        $log_debug("Hook Event Addr: {}", (void*)&event);

        EventHandler::emit(event);

        $log_debug("Is cancelled: {}", event.isCancelled());

        if (event.isCancelled()) {
            return;
        } else {
            return _mouse_device_feed_hook.call(mouseDevice, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
        }
    }

    InlineHook<HRESULT(HWND, UINT, WPARAM, LPARAM)> _window_process_hook;
    HRESULT _window_process_detour(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_KEYUP) {
            UINT vk = static_cast<UINT>(wParam);

            if (vk == VK_TAB) {
                KeyboardEvent event{
                    .key = VK_TAB,
                    .isDown = false,
                };

                EventHandler::emit(event);
            }
        }

        return _window_process_hook.call(hwnd, msg, wParam, lParam);
    }

    void init() {
        HookManager::createInlineHook(_mouse_device_feed_hook.getHook(), (void*)$get_address("MouseDevice::feed"), &_mouse_device_feed_detour);
        // HookManager::createInlineHook(_keyboard_feed_hook.getHook(), (void*)$get_address("Keyboard::feed"), &_keyboard_feed_detour);
        HookManager::createInlineHook(_window_process_hook.getHook(), (void*)$get_address("MainWindow::_windowProc"), &_window_process_detour);
    }
}