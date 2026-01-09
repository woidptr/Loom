#pragma once
#include <Windows.h>
#include "../Hook.hpp"

struct WindowProcHookTag {};

class WindowProcHook : public InlineHook<WindowProcHookTag, LRESULT(HWND, UINT, WPARAM, LPARAM)> {
public:
    WindowProcHook() : InlineHook($get_signature("MainWindow::_windowProc")) {
        this->hook();
    }
};