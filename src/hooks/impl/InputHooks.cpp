#include "InputHooks.hpp"
#include <GameInput.h>
#include <hooks/HookManager.hpp>
#include <hooks/InlineHook.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/input/MouseEvent.hpp>

typedef HRESULT(WINAPI* PFN_GAME_INPUT_CREATE)(IGameInput** gameInput);

// Typedef for the function we want to hook (GetCurrentReading)
typedef HRESULT(WINAPI* GetCurrentReading_t)(IGameInput* pThis,
    GameInputKind inputKind,
    IGameInputDevice* device,
    IGameInputReading** reading);

namespace InputHooks {
    InlineHook<void(int16_t, bool)> _keyboard_feed_hook;
    void _keyboard_feed_detour(int16_t key, bool isDown) {
        KeyboardEvent event{
            .key = key,
            .isDown = isDown,
        };

        EventHandler::emit(event);

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

        EventHandler::emit(event);

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

    InlineHook<HRESULT(IGameInput*, GameInputKind, IGameInputDevice*, IGameInputReading**)> _reading_hook;
    HRESULT WINAPI _reading_detour(IGameInput* pThis, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading)
    {
        // Call original to let the OS populate the input
        // HRESULT hr = oGetCurrentReading(pThis, inputKind, device, reading);

        // Now *reading contains the input data! 
        // You can read it here (or hook the reading's vtable if you need to spoof it).

        // return hr;

        $log_debug("Test");

        return _reading_hook.call(pThis, inputKind, device, reading);
    }

    void init() {
        HMODULE hGameInput = GetModuleHandleA("gameinput.dll");

        if (!hGameInput) {
            $log_debug("hGameInput not found!");
            return;
        }

        PFN_GAME_INPUT_CREATE pGameInputCreate = (PFN_GAME_INPUT_CREATE)GetProcAddress(hGameInput, "GameInputCreate");
        if (!pGameInputCreate) {
            $log_debug("pGameInputCreate not found!");
            return;
        }

        IGameInput* dummyInput = nullptr;
        if (FAILED(pGameInputCreate(&dummyInput))) return;

        void** vTable = *(void***)dummyInput;

        void* pGetCurrentReading = vTable[3];

        dummyInput->Release();

        HookManager::createInlineHook(_mouse_device_feed_hook.getHook(), (void*)$get_address("MouseDevice::feed"), &_mouse_device_feed_detour);
        // HookManager::createInlineHook(_keyboard_feed_hook.getHook(), (void*)$get_address("Keyboard::feed"), &_keyboard_feed_detour);
        HookManager::createInlineHook(_window_process_hook.getHook(), (void*)$get_address("MainWindow::_windowProc"), &_window_process_detour);
        HookManager::createInlineHook(_reading_hook.getHook(), pGetCurrentReading, &_reading_detour);
    }
}